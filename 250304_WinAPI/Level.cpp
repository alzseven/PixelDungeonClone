#include "Level.h"
#include "DungeonGenerator.h"
#include "config.h"
#include <random>

#include "CommonFunction.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "Entity.h"
#include "IntegratedDungeonSystem.h"
#include "Monster.h"
#include "Player.h"
#include "TileVariationManager.h"
#include "TurnManager.h"
#include "Camera.h"
#include "FieldOfView.h"
#include "FModSoundPlayer.h"
#include "IDungeonGenerator.h"
#include "UIManager.h"
#include "UI/Test/UITestView.h"
#include "Item.h"
#include "HealPotion.h"
#include "TileMapping8x8.h"


void Level::Init(Player* player, int floor, bool isProcedural)
{
    this->player = player;
    levelFloor = floor;
    turnManager = new TurnManager();

    sampleTile = D2DImageManager::GetInstance()->AddImage(
        "배틀시티_샘플타일", L"Image/tiles_sewers.png", 
        16, 16);

    wallTile = D2DImageManager::GetInstance()->AddImage("wallTile", L"Image/tiles_sewers.png", 32, 32);

    // blurImage = D2DImageManager::GetInstance()->AddImage("blur", L"Image/blur.png");
    // shadowImage = D2DImageManager::GetInstance()->AddImage("shadow", L"Image/shadow.png");
    //
    
    for (int i = 0; i < TILE_Y; ++i)
    {
        for (int j = 0; j < TILE_X; ++j)
        {
            tempTile[TILE_X * i + j] =
            {
                GRID_POS_OFFSET.x + j * TILE_SIZE,
                GRID_POS_OFFSET.y + i * TILE_SIZE,
                GRID_POS_OFFSET.x + (j + 1) * TILE_SIZE,
                GRID_POS_OFFSET.y + (i + 1) * TILE_SIZE
            };
        }
    }

    mapRc = {tempTile[0].left, tempTile[0].top, tempTile[TILE_X * TILE_Y - 1].right, tempTile[TILE_X * TILE_Y - 1].bottom};
    
    for (auto& s : shouldBeRender)
    {
        s = true;
    }
    for (auto& h : hasExplored)
    {
        h = true;
    }
    for (auto& i : isSeen)
    {
        i = true;
    }

    FileLoad();

    // Set map dimensions
    mapWidth = TILE_X;
    mapHeight = TILE_Y;


    // Generate dungeon
    dungeonSystem.GenerateDungeon(true, this, mapWidth, mapHeight, 10, 8, 12, nullptr, floor);

    ascInd = dungeonSystem.GetDungeonGenerator()->GetAscInd();
    int ascY = ascInd / TILE_X;
    int ascX = ascInd % TILE_X;
    ascCenter = GetPosByGridIndex(ascX, ascY);
    descInd = dungeonSystem.GetDungeonGenerator()->GetDescInd();
    int descY = descInd / TILE_X;
    int descX = descInd % TILE_X;
    descCenter = GetPosByGridIndex(descX, descY);

    //player->SetStairs(ascCenter, descCenter);
    /*ascRc = {
        static_cast<long>(ascCenter.x) - TILE_SIZE / 2,
        static_cast<long>(ascCenter.y) - TILE_SIZE / 2,
        static_cast<long>(ascCenter.x) + TILE_SIZE / 2,
        static_cast<long>(ascCenter.y) + TILE_SIZE / 2,

    };
    descRc = {
        static_cast<long>(descCenter.x) - TILE_SIZE / 2,
        static_cast<long>(descCenter.y) - TILE_SIZE / 2,
        static_cast<long>(descCenter.x) + TILE_SIZE / 2,
        static_cast<long>(descCenter.y) + TILE_SIZE / 2,

    };*/

    // Place player near entrance
    playerInitP = GetEntranceSpawnPosition();
    //player->SetPosition(playerPos);
    AddActor(player);
    
    camera = new Camera();
    camera->Init(playerInitP);

    // UI
    uiManager = UIManager::GetInstance();
    uiManager->RegisterCamera(camera);
    //
    
    for (auto actor : actors)
    {
        if (actor)
        {
            turnManager->AddActor(actor);
            uiManager->RegisterEntity(actor);
        }
    }
    turnManager->Init();


    // Item
    Item* potion1 = new HealPotion(playerInitP + FPOINT{ TILE_SIZE , TILE_SIZE });
    Item* potion2 = new HealPotion(playerInitP + FPOINT{ TILE_SIZE , 0 });
    AddItem(potion1);
    AddItem(potion2);

    FModSoundPlayer::GetInstance()->Play("descend");
    FModSoundPlayer::GetInstance()->Play("sewers_1");
}

void Level::Release()
{
    /*if (uiManager)
    {
        uiManager->DeleteLevelUI();
        uiManager = nullptr;
    }*/
    
	for (auto& actor : actors)
	{
		if (actor)
		{
			delete actor;
			actor = nullptr;
		}
	}
    
    if (player)
    {
        //delete player;
        player = nullptr;
        //uiManager->SetCurrentPlayer(nullptr);
    }

    // Items
    for (auto& item : items)
    {
        if (item)
        {
            delete item;
            item = nullptr;
        }
    }

    if (camera) {
        camera = nullptr;
    }

}

void Level::Update()
{
    uiManager->Update();

    /*if (KeyManager::GetInstance()->IsOnceKeyDown('W')) {
        Ascending();
    }
    if (KeyManager::GetInstance()->IsOnceKeyDown('S')) {
        Descending();
    }*/

    /*if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE)) {
        player->TakeDamage(30);
    }*/
    
    if (player) {
        if (player->GetState() == EntityState::MOVE) {
            camera->UpdateCenter(player->GetPosition());
        }
        else {
            camera->Update();
        }
    }
    
  
	for (int i = 0; i < TILE_Y; ++i)
	{
		for (int j = 0; j < TILE_X; ++j)
		{
			if (RectInRect(tempTile[TILE_X * i + j], camera->GetScreen())) {
                shouldBeRender[TILE_X * i + j] = true;
			}
			else {
                shouldBeRender[TILE_X * i + j] = false;
			}

		}
	}


    POINT ConvertedDragEndP = {
        camera->ConvertToWorldX(MouseManager::GetInstance()->GetClickP().x),
        camera->ConvertToWorldY(MouseManager::GetInstance()->GetClickP().y)
    };

    if (PtInRect(&mapRc, ConvertedDragEndP))
    {
        if (MouseManager::GetInstance()->GetClickValueUsed() == false)
        {
            long posX = ConvertedDragEndP.x;
            long posY = ConvertedDragEndP.y;

            long indX = (posX - mapRc.left) / TILE_SIZE;
            long indY = (posY - mapRc.top) / TILE_SIZE;

            if (indX >= 0 && indX < TILE_X && indY >= 0 && indY < TILE_Y) ///
            {
                /// 구현 하고 싶은 로직 넣는 부분
                //map[indY * TILE_X + indX].type = TT::COUNT;
                
                // 현재 플레이어 위치 재클릭 했을 때, 아이템 줍기
                if (GetPosByGridIndex(indX, indY) == player->GetPosition())
                {
                    Item* item = GetItemAt(GetPosByGridIndex(indX, indY));
                    if (item)
                    {
                        player->GetItem(item);
                        MoveItemToInven(item);
                    }
                }
                // 플레이어 도착지 설정

                if (map[indY * TILE_X + indX].CanGo() && map[indY * TILE_X + indX].explored)
                    player->SetNextPos(GetPosByGridIndex(indX, indY));
                    //player->SetJustMoved(false);
            } ///

            MouseManager::GetInstance()->InitPoints();
            MouseManager::GetInstance()->AlreadyClickUsed();
        }
    } ///디버깅을 위해 마우스 왼쪽 버튼을 떼면 그 자리에 있는 타일이 빨간색으로 변하게 해놨습니다. 
	  ///맵으로 사용하실 땐 타일 선택 로직(이동 및 공격)을 써주세요!


    SetVisibleTile();
    

    for (auto actor : actors)
    {
        if (actor) actor->Update();
    }
    turnManager->ProcessTurns(this);
}

void Level::Render(HDC hdc)
{
    // 8x8 타일 렌더링 사용
    for (int i = 0; i < TILE_Y; ++i) {
        for (int j = 0; j < TILE_X; ++j) {
            if (!shouldBeRender[TILE_X * i + j]) continue;

            bool isExplored = map[TILE_X * i + j].explored;
            bool isVisible = map[TILE_X * i + j].visible;
            int rawType = map[TILE_X * i + j].type;
            int tileType = rendermap[TILE_X * i + j];
            int tileX = camera->ConvertToRendererX(tempTile[TILE_X * i + j].left);
            int tileY = camera->ConvertToRendererY(tempTile[TILE_X * i + j].top);

            if (isExplored)
            {
                // 벽 타일인 경우 세부 렌더링 사용
                if (rawType == 0) {
                    // 벽 타일 렌더링
                    dungeonSystem.GetDungeonGenerator()->RenderWallTile(
                        wallTile, j, i, tileX, tileY, 
                        camera->GetZoomScale() * 2.f, mapData
                    );
                } else {
                    // 기존 방식으로 다른 타일 렌더링
                    POINT frame = GetCurrentFrame(tileType);
                    sampleTile->RenderFrameScale(tileX, tileY, 
                        camera->GetZoomScale() * 2.f,
                        camera->GetZoomScale() * 2.f, frame.x, frame.y);
                }

                if (isVisible == false)
                {
                    // if (blurImage)
                    // {                    // 시야 효과 적용
                    //     blurImage->RenderFrameScale(tileX, tileY, 
                    //         camera->GetZoomScale() * 2.f,
                    //         camera->GetZoomScale() * 2.f, 0, 0);
                    // }
                    if (sampleTile)
                    {
                        // 시야 효과 적용
                        sampleTile->RenderFrameScale(tileX, tileY, 
                            camera->GetZoomScale() * 2.f,
                            camera->GetZoomScale() * 2.f, 0, 9, 0, false, false, 0.5f);
                    }

                }
            }
            else
            {

                
                if (sampleTile)
                {
                    // 시야 효과 적용
                    sampleTile->RenderFrameScale(tileX, tileY, 
                        camera->GetZoomScale() * 2.f,
                        camera->GetZoomScale() * 2.f, 0, 9);
                }

            }
        }
    }
    // for (int i = 0; i < TILE_Y; ++i)
    // {
    //     for (int j = 0; j < TILE_X; ++j)
    //     {
    //         if (!shouldBeRender[TILE_X * i + j]) continue;
    //
    //         bool isVisible = map[TILE_X * i + j].visible;
    //         int tileType = rendermap[TILE_X * i + j];
    //         int tileX = camera->ConvertToRendererX(tempTile[TILE_X * i + j].left);
    //         int tileY = camera->ConvertToRendererY(tempTile[TILE_X * i + j].top);
    //
    //         // Get the current frame coordinates for this tile type
    //         POINT frame = GetCurrentFrame(tileType);
    //
    //         // Render the tile using the frame coordinates
    //         sampleTile->RenderFrameScale(tileX, tileY, camera->GetZoomScale() * 2.f,
    //                                             camera->GetZoomScale() * 2.f, frame.x, frame.y);
    //         sampleTile->RenderFrameScale(tileX, tileY, camera->GetZoomScale() * 2.f,
    //                                 camera->GetZoomScale() * 2.f, 0, 9, 0, false, false, isVisible ? 0.f : 0.5f);
    //     }
    // }
    
    //Render Items
    for (auto item : items)
    {
        if (map[GetMapIndex(item->GetPosition().x, item->GetPosition().y)].visible)
        {
            // if (shadowImage)
            // {
            //     shadowImage->Middle_RenderFrameScale(
            //         camera->ConvertToRendererX(item->GetPosition().x),
            //             camera->ConvertToRendererY(item->GetPosition().y),
            //             camera->GetZoomScale() * 4.f, camera->GetZoomScale() * 4.f,
            //             0, 0);
            // }
            
            D2DImage* image = item->GetImage();
            if (image) {
                image->
                    Middle_RenderFrameScale(
                        camera->ConvertToRendererX(item->GetPosition().x),
                        camera->ConvertToRendererY(item->GetPosition().y),
                        camera->GetZoomScale() * 2.f, camera->GetZoomScale() * 2.f,
                        item->GetImgIdX(), item->GetImgIdY());
            }
        }
    }


    // Render actors
    for (auto actor : actors)
    {
		//if (map[GetMapIndex(actor->GetPosition().x, actor->GetPosition().y)].visible)
		{
		    // if (shadowImage)
		    // {
		    //     shadowImage->Middle_RenderFrameScale(
      //               camera->ConvertToRendererX(actor->GetPosition().x),
      //                   camera->ConvertToRendererY(actor->GetPosition().y),
      //                   camera->GetZoomScale() * 4.f, camera->GetZoomScale() * 4.f,
      //                   0, 0);
		    // }
		    
			if (actor->GetImage()) {
				actor->GetImage()->
					Middle_RenderFrameScale(
						camera->ConvertToRendererX(actor->GetPosition().x), 
						camera->ConvertToRendererY(actor->GetPosition().y),
						camera->GetZoomScale() * 2.f, camera->GetZoomScale() * 2.f, actor->GetCurAnimIdx(), 0);
			}
		}
	}
    // UI
    uiManager->Render();
}

void Level::FileLoad()
{
    // 파일 로드
    HANDLE hFile = CreateFile(
        L"TileMapData.dat", GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(g_hWnd, TEXT("파일 열기 실패"), TEXT("경고"), MB_OK);
        return;
    }
    DWORD dwByte = 0;
    if (!ReadFile(hFile, map, sizeof(map), &dwByte, NULL))
    {
        MessageBox(g_hWnd, TEXT("파일 읽기 실패"), TEXT("경고"), MB_OK);
    }
    CloseHandle(hFile);
}

Level::Level()
{
}

Level::~Level()
{
    Release();
}

FPOINT Level::GetPlayerPos()
{
    return player->GetPosition();
}


Entity* Level::GetActorAt(FPOINT pos)
{
    if (actors.empty()) return nullptr;

    for (auto actor : actors)
    {
        if (actor && actor->GetPosition() == pos)
            return actor;
    }
    return nullptr;
}

Item* Level::GetItemAt(FPOINT pos)
{
    if (items.empty()) return nullptr;

    for (auto item : items)
    {
        if (item && item->GetPosition() == pos)
            return item;
    }
    return nullptr;
}

void Level::MoveItemToInven(Item* item)
{
    auto it = find(items.begin(), items.end(), item);
    if (it != items.end())
        items.erase(it);
}

void Level::AddActor(Entity* actor)
{
    // 추가하려는 Entity가 이미 container에 있다면 return
    auto it = find(actors.begin(), actors.end(), actor);
    if (it != actors.end())
        return;

    actors.push_back(actor);
}

void Level::AddItem(Item* item)
{
    auto it = find(items.begin(), items.end(), item);
    if (it != items.end())
        return;

    items.push_back(item);
}

void Level::GenerateMap(int width, int height)
{
    // 맵 생성
    // mapData = dungeonGenerator.Generate(width, height);

    // Map 객체 배열 초기화 (만약 사용한다면)
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;
            // Map 객체 설정
            map[index].type = mapData[y][x];
        }
    }

    // 생성된 맵을 기반으로 tempTile 및 shouldBeRender, hasExplored, isSeen 배열 초기화
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;
            // 맵 타일 위치 설정 (기존 코드 참고)
            tempTile[index] = {
                GRID_POS_OFFSET.x + x * TILE_SIZE / 2,
                GRID_POS_OFFSET.y + y * TILE_SIZE / 2,
                GRID_POS_OFFSET.x + (x + 1) * TILE_SIZE / 2,
                GRID_POS_OFFSET.y + (y + 1) * TILE_SIZE / 2
            };

            // 타일 속성 초기화
            shouldBeRender[index] = true;
            hasExplored[index] = false;
            isSeen[index] = false;
        }
    }

    // 맵 경계 설정
    mapRc = {
        tempTile[0].left,
        tempTile[0].top,
        tempTile[(height - 1) * width + (width - 1)].right,
        tempTile[(height - 1) * width + (width - 1)].bottom
    };
}

int Level::GetTileType(int x, int y) const
{
    // 좌표가 맵 범위 내인지 확인
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
    {
        return mapData[y][x];
    }
    return 0; // 맵 범위 밖은 벽으로 처리
}

bool Level::IsSolid(int x, int y) const
{
    int tileType = GetTileType(x, y);
    return tileType == 0; // TILE_WALL인 경우 통과 불가
}

FPOINT Level::GetRandomFloorTile() const
{
    std::vector<FPOINT> floorTiles;

    // Check if map is initialized
    if (mapData.empty() || mapHeight <= 0 || mapWidth <= 0)
    {
        // Return a default position if map is not initialized
        return {
            static_cast<float>(GRID_POS_OFFSET.x + TILE_SIZE),
            static_cast<float>(GRID_POS_OFFSET.y + TILE_SIZE)
        };
    }

    // 모든 바닥 타일 위치 수집
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            int tileType = mapData[y][x];
            // Check for any floor tile type (including variations)
            if (tileType == 1 ) // TILE_FLOOR_NORMAL to TILE_FLOOR_MOSSY
            {
                FPOINT pos;
                pos.x = static_cast<float>(GRID_POS_OFFSET.x + x * TILE_SIZE + TILE_SIZE / 2);
                pos.y = static_cast<float>(GRID_POS_OFFSET.y + y * TILE_SIZE + TILE_SIZE / 2);
                floorTiles.push_back(pos);
            }
        }
    }

    // If no floor tiles found, return a default position
    if (floorTiles.empty())
    {
        return {
            static_cast<float>(GRID_POS_OFFSET.x + TILE_SIZE),
            static_cast<float>(GRID_POS_OFFSET.y + TILE_SIZE)
        };
    }

    // 랜덤하게 하나 선택
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, floorTiles.size() - 1);
    return floorTiles[dis(gen)];
}

// Add GetCurrentFrame implementation
POINT Level::GetCurrentFrame(int tileType) const
{
    // Shattered Pixel Dungeon 타일맵 에셋과 매핑
    switch (tileType)
    {
    // 기본 타일
    case IDungeonGenerator::TILE_WALL:
        return {0, 3}; // 일반 벽 타일
    case IDungeonGenerator::TILE_FLOOR:
        return {0, 0}; // 일반 바닥 타일
    case IDungeonGenerator::TILE_DOOR:
        return {8, 3}; // 문
    case IDungeonGenerator::TILE_ENTRANCE:
        return {0, 1}; // 입구
    case IDungeonGenerator::TILE_EXIT:
        return {1, 1}; // 출구
    case IDungeonGenerator::TILE_HIDDEN_DOOR:
        return {13, 3}; // 숨겨진 문
    case IDungeonGenerator::TILE_NONE:
        return {0, 9}; // 빈 공간
        
    // 벽 변형 타일
    case IDungeonGenerator::TILE_WALL_TOP:
        return {0, 6}; // 상단 벽(위쪽이 바닥)
    case IDungeonGenerator::TILE_WALL_BOTTOM:
        return {5, 8}; // 하단 벽(아래쪽이 바닥)
    case IDungeonGenerator::TILE_WALL_LEFT:
        return {2, 9}; // 좌측 벽(왼쪽이 바닥)
    case IDungeonGenerator::TILE_WALL_RIGHT:
        return {4, 9}; // 우측 벽(오른쪽이 바닥)
        
    // 코너 타일
    case IDungeonGenerator::TILE_WALL_CORNER_TL:
        return {9, 6}; // 상단 좌측 코너(북서쪽 코너)
    case IDungeonGenerator::TILE_WALL_CORNER_TR:
        return {1, 13}; // 상단 우측 코너(북동쪽 코너)
    case IDungeonGenerator::TILE_WALL_CORNER_BL:
        return {3, 13}; // 하단 좌측 코너(남서쪽 코너)
    case IDungeonGenerator::TILE_WALL_CORNER_BR:
        return {5, 13}; // 하단 우측 코너(남동쪽 코너)
        
    // 모서리 타일
    case IDungeonGenerator::TILE_WALL_TOP_LEFT:
        return {0, 7}; // 상단 좌측 모서리(위쪽과 왼쪽이 바닥)
    case IDungeonGenerator::TILE_WALL_TOP_RIGHT:
        return {2, 7}; // 상단 우측 모서리(위쪽과 오른쪽이 바닥)
    case IDungeonGenerator::TILE_WALL_BOTTOM_LEFT:
        return {1, 9}; // 하단 좌측 모서리(아래쪽과 왼쪽이 바닥)
    case IDungeonGenerator::TILE_WALL_BOTTOM_RIGHT:
        return {8, 9}; // 하단 우측 모서리(아래쪽과 오른쪽이 바닥)
        
    // 내부 벽 및 특수 타일
    case IDungeonGenerator::TILE_WALL_INNER:
        return {1, 8}; // 내부 벽(모든 방향이 벽)
    case IDungeonGenerator::TILE_WALL_SOLO:
        return {3, 8}; // 독립된 벽(주변에 벽이 없음)
    case IDungeonGenerator::TILE_WALL_SIDE_TOP:
        return {6, 9}; // 측면 상단(위쪽과 아래쪽이 바닥)
    case IDungeonGenerator::TILE_WALL_SIDE_BOTTOM:
        return {7, 9}; // 측면 하단(왼쪽과 오른쪽이 바닥)
    case IDungeonGenerator::TILE_WALL_SIDE_LEFT:
        return {3, 9}; // 측면 좌측(왼쪽이 바닥, 위아래가 벽)
    case IDungeonGenerator::TILE_WALL_SIDE_RIGHT:
        return {5, 9}; // 측면 우측(오른쪽이 바닥, 위아래가 벽)
        
    // 바닥 변형 타일
    case IDungeonGenerator::TILE_FLOOR_NORMAL:
        return {0, 0}; // 일반 바닥
    case IDungeonGenerator::TILE_FLOOR_FANCY:
        return {1, 0}; // 장식된 바닥
    case IDungeonGenerator::TILE_FLOOR_CRACKED:
        return {2, 0}; // 금이 간 바닥
    case IDungeonGenerator::TILE_FLOOR_MOSSY:
        return {6, 0}; // 이끼 낀 바닥
        
    // 기본값 (알 수 없는 타일 타입)
    default:
        return {1, 4}; // 기본 타일 (알 수 없는 타일 타입)
    }
}

FPOINT Level::GetEntranceSpawnPosition() const
{
    // Find entrance position
    std::pair<int, int> entrancePos = {-1, -1};
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (mapData[y][x] == 3)
            {
                entrancePos = {x, y};
                return GetPosByGridIndex(x, y);
                break;
            }
        }
        if (entrancePos.first != -1) break;
    }

    // If no entrance found, return a default position
    if (entrancePos.first == -1)
    {
        return GetRandomFloorTile();
    }

    // Look for a floor tile near the entrance
    const int SAFE_RADIUS = 2; // Safe radius around entrance
    std::vector<std::pair<int, int>> validPositions;

    for (int dy = -SAFE_RADIUS; dy <= SAFE_RADIUS; dy++)
    {
        for (int dx = -SAFE_RADIUS; dx <= SAFE_RADIUS; dx++)
        {
            int newX = entrancePos.first + dx;
            int newY = entrancePos.second + dy;

            // Check bounds
            if (newX < 0 || newX >= mapWidth || newY < 0 || newY >= mapHeight)
            {
                continue;
            }

            // Check if it's a floor tile
            if (mapData[newY][newX] == TileVariationManager::TILE_FLOOR)
            {
                validPositions.push_back({newX, newY});
            }
        }
    }

    // If no valid positions found, return a random floor tile
    if (validPositions.empty())
    {
        return GetRandomFloorTile();
    }

    // Choose a random valid position
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validPositions.size() - 1);
    auto chosenPos = validPositions[dis(gen)];

    // Convert to world coordinates
    return GetPosByGridIndex(chosenPos.first, chosenPos.second);
}

// // 맵 함수-이 주소로 세팅. 레벨이 구현되면 레벨에서 이 작업하면 될 것같음.
// FPOINT Level::GetRandomFloorTile()
// {
//     // 랜덤한 타일의 center 좌표를 반환
//     // 벽이면 반환하면 안됨
//     // 플레이어 주변도 X
//
//     RECT rc;
//     rc.left = 0;
//     rc.right = TILE_X * 2;
//     rc.top = 0;
//     rc.bottom = TILE_Y * 2;
//
//
//     int x = (rand() % TILE_SIZE);
//     int y = (rand() % TILE_SIZE);
//
//     //TODO: Check visible with function
//     while ((map[y * TILE_X + x].type == 0) || RectInRect(rc, map[(int)y][(int)x].rc))
//     {
//         x = (rand() % TILE_SIZE);
//         y = (rand() % TILE_SIZE);
//     }
// 			
//     FPOINT pos = { map[y][x].center.x, map[y][x].center.y };
//
//     return pos;
// }
//
// void Level::SetEntityPos()
// {
//     // entity 배치
//     FPOINT pos = GetRandomFloorTile();
//     astarGame->SetEntityOnMap(pos);
// }

void Level::ResetVisibleTile()
{
    // 시야 리셋-모든 타일 검사. 맵이 커지면 비효율적일수도 있을 것 같음.
    for (int i = 0; i < TILE_Y; i++)
    {
        for (int j = 0; j < TILE_X; j++)
        {
            if (map[i * TILE_X + j].visible == true)
            {
                map[i * TILE_X + j].visible = false;
                // if (map[i * TILE_X + j].type == 0/* ||
                //     map[i][j].GetType() == AstarTileType::Start ||
                //     map[i][j].GetType() == AstarTileType::End*/)
                // {
                //     map[i * TILE_X + j].visible = false;
                // }
            }
        }
    }
}

void Level::SetVisibleTile()
{
    //// 시야 설정-시야에 드는 타일은 색을 다르게 함.
    //// 시야 리셋
    ResetVisibleTile();
    
    int index = GetMapIndex(player->GetPosition().x, player->GetPosition().y);
    int pTileXIndex = index % TILE_Y;
    int pTileYIndex = index / TILE_Y;
    map[index].visible = true;
    if (map[index].explored == false) map[index].explored = true;
    
    for (auto scanDirection : scanDirections)
    {
        fov->Calculate(reinterpret_cast<Map(&)[TILE_Y][TILE_X]>(map), viewRange, pTileXIndex, pTileYIndex, 0,
                       1.0f, 0.0f, scanDirection);
    }
}


void Level::Render8x8Tiles(HDC hdc)
{
    // 기본 TILE_SIZE는 90으로 유지
    // const int TILE_SIZE = 90;
    // 8x8 타일 크기는 기본 타일 크기의 절반
    const int SUBTILE_SIZE = TILE_SIZE / 2;
    
    // TileMapping8x8 인스턴스 생성
    TileMapping8x8 tileMapping;
    
    // 16x16 맵을 8x8 맵으로 변환
    std::vector<std::vector<int>> map8x8 = tileMapping.ApplyEdgeTiles(mapData);
    
    // 맵 크기 계산
    int height8x8 = map8x8.size();
    int width8x8 = map8x8[0].size();
    
    // 8x8 타일 렌더링
    for (int y = 0; y < height8x8; y++)
    {
        for (int x = 0; x < width8x8; x++)
        {
            // 타일 ID 가져오기
            int tileID = map8x8[y][x];
            
            // 타일 위치 계산 (SUBTILE_SIZE 사용)
            int tileX = GRID_POS_OFFSET.x + x * SUBTILE_SIZE;
            int tileY = GRID_POS_OFFSET.y + y * SUBTILE_SIZE;
            
            // 화면 밖의 타일은 렌더링하지 않음
            if (!shouldBeRender[y / 2 * TILE_X + x / 2]) continue;
            
            // 타일 가시성 확인
            bool isVisible = map[y / 2 * TILE_X + x / 2].visible;
            
            // 타일 이미지 위치 가져오기
            POINT frame = tileMapping.GetImagePosition(tileID);
            
            // 타일 렌더링 (SUBTILE_SIZE에 맞게 스케일 조정)
            sampleTile->RenderFrameScale(
                camera->ConvertToRendererX(tileX),
                camera->ConvertToRendererY(tileY),
                camera->GetZoomScale() * 2.f *(SUBTILE_SIZE / 16.0f),
                camera->GetZoomScale()* 2.f * (SUBTILE_SIZE / 16.0f),
                frame.x, frame.y);
            
            // 가시성에 따른 어두운 오버레이 렌더링
            if (!isVisible) {
                sampleTile->RenderFrameScale(
                    camera->ConvertToRendererX(tileX),
                    camera->ConvertToRendererY(tileY),
                    camera->GetZoomScale() * 2.f* (SUBTILE_SIZE / 16.0f),
                    camera->GetZoomScale() * 2.f* (SUBTILE_SIZE / 16.0f),
                    0, 9, 0, false, false, 0.5f);
            }
        }
    }
}
// unsigned char CalculateBitmask(const std::vector<std::vector<int>>& map, int x, int y) {
//     // 8방향 오프셋 (시계 방향으로 좌상, 상, 우상, 우, 우하, 하, 좌하, 좌)
//     const int dx[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
//     const int dy[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
//     
//     unsigned char bitmask = 0;
//     
//     for (int i = 0; i < 8; i++) {
//         int nx = x + dx[i];
//         int ny = y + dy[i];
//         
//         // 맵 범위 확인
//         if (nx < 0 || nx >= map[0].size() || ny < 0 || ny >= map.size()) {
//             // 맵 밖은 벽으로 처리
//             bitmask |= (1 << i);
//             continue;
//         }
//         
//         // 벽 타일 확인
//         if (IsWall(map[ny][nx])) {
//             bitmask |= (1 << i);
//         }
//     }
//     
//     return bitmask;
// }

void TestLevel::Init(Player* player, int floor, bool isProcedural)
{
    turnManager = new TurnManager();

    sampleTile = D2DImageManager::GetInstance()->AddImage(
        "배틀시티_샘플타일", L"Image/tiles_sewers.png",
        16, 16);

    wallTile = D2DImageManager::GetInstance()->AddImage("wallTile", L"Image/tiles_sewers.png", 32, 32);

    for (int i = 0; i < TILE_Y; ++i)
    {
        for (int j = 0; j < TILE_X; ++j)
        {
            tempTile[TILE_X * i + j] =
            {
                GRID_POS_OFFSET.x + j * TILE_SIZE,
                GRID_POS_OFFSET.y + i * TILE_SIZE,
                GRID_POS_OFFSET.x + (j + 1) * TILE_SIZE,
                GRID_POS_OFFSET.y + (i + 1) * TILE_SIZE
            };
        }
    }

    mapRc = { tempTile[0].left, tempTile[0].top, tempTile[TILE_X * TILE_Y - 1].right, tempTile[TILE_X * TILE_Y - 1].bottom };

    for (auto& s : shouldBeRender)
    {
        s = true;
    }
    for (auto& h : hasExplored)
    {
        h = true;
    }
    for (auto& i : isSeen)
    {
        i = true;
    }

    FileLoad();

    // Set map dimensions
    mapWidth = TILE_X;
    mapHeight = TILE_Y;


    // Generate dungeon
    dungeonSystem.GenerateDungeon(false, this, mapWidth, mapHeight, 10, 8, 12, filePath);

    // Place player near entrance
    FPOINT playerPos = GetEntranceSpawnPosition();
    player = new Player(playerPos, 1000.f, 20, 50, 2);
    AddActor(player);

    camera = new Camera();
    camera->Init(player->GetPosition());

    // UI
    uiManager = UIManager::GetInstance();
    uiManager->Init();
    uiManager->RegisterCamera(camera);
    uiManager->RegisterPlayer(player);
    //

    for (auto actor : actors)
    {
        if (actor)
        {
            turnManager->AddActor(actor);
            uiManager->RegisterEntity(actor);
        }
    }
    turnManager->Init();


    // Item
    Item* potion1 = new HealPotion(playerPos + FPOINT{ TILE_SIZE , TILE_SIZE });
    Item* potion2 = new HealPotion(playerPos + FPOINT{ TILE_SIZE , 0 });
    AddItem(potion1);
    AddItem(potion2);
}
