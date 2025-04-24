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
#include "UIManager.h"
#include "UI/Test/UITestView.h"
#include "Item.h"
#include "HealPotion.h"



void Level::Init()
{
    turnManager = new TurnManager();

   /* camera = new Camera();
    camera->Init();*/

    sampleTile = D2DImageManager::GetInstance()->AddImage(
        "배틀시티_샘플타일", L"Image/tiles_sewers.png",
        16, 16);


    tempTileSize = TILE_SIZE;

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

    // BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
    // GreyBrush = CreateSolidBrush(RGB(100, 100, 100));
    // WhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    // RedBrush = CreateSolidBrush(RGB(255, 0, 0));


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
    dungeonSystem.GenerateDungeon(this, mapWidth, mapHeight, 10, 8, 12);

    // Place player near entrance
    FPOINT playerPos = GetEntranceSpawnPosition();
    //TODO:MergeCheck
    //   Player* player = new Player(playerPos, 50.0f);

    // 시작 위치 테스트용 매직넘버
    player = new Player(playerPos, 1000.f, 20, 5, 2);
    // Entity* monster1 = new Monster(GetPosByGridIndex(5, 4), 300.f, 15, 4, 3);
    // Entity* monster2 = new Monster(GetPosByGridIndex(4, 5), 300.f, 15, 4, 3);

    AddActor(player);
    // AddActor(monster1);
    // AddActor(monster2);
    
    camera = new Camera();
    camera->Init(player->GetPosition());
    
    for (auto actor : actors)
    {
        if (actor)
            turnManager->AddActor(actor);
    }
    turnManager->Init();



    // Item
    Item* potion1 = new HealPotion(playerPos + FPOINT{ TILE_SIZE , TILE_SIZE });
    Item* potion2 = new HealPotion(playerPos + FPOINT{ TILE_SIZE , 0 });
    AddItem(potion1);
    AddItem(potion2);
    // UI
    uiManager = UIManager::GetInstance();
    uiManager->Init();
    uiManager->RegisterPlayer(player);
    uiManager->RegisterEntity(player);
}

void Level::Release()
{
    
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
        uiManager->SetCurrentPlayer(nullptr);
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
    
    uiManager = nullptr;

    if (camera) {
        camera = nullptr;
    }

}

void Level::Update()
{
    uiManager->Update();

    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE)) {
        player->TakeDamage(30);
    }
    
    if (player->GetState() == EntityState::MOVE) {
        camera->UpdateCenter(player->GetPosition());
    }
    else {
        camera->Update();
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
                if (map[indY * TILE_X + indX].CanGo())
                    dynamic_cast<Player*>(player)->SetNextPos(GetPosByGridIndex(indX, indY));
            }

            MouseManager::GetInstance()->InitPoints();
            MouseManager::GetInstance()->AlreadyClickUsed();
        }
    } ///디버깅을 위해 마우스 왼쪽 버튼을 떼면 그 자리에 있는 타일이 빨간색으로 변하게 해놨습니다. 
	  ///맵으로 사용하실 땐 타일 선택 로직(이동 및 공격)을 써주세요!

    SetVisibleTile();

    /*if (MouseManager::GetInstance()->GetIsDragging(MOUSE_LEFT)) //카메라 도입 이후부턴 이거 넣으면 안됩니다!
    {
        long tempDeltaX = MouseManager::GetInstance()->GetDeltaX();
        long tempDeltaY = MouseManager::GetInstance()->GetDeltaY();

        for (auto& t : tempTile)
        {
            t.left += tempDeltaX;
            t.right += tempDeltaX;
            t.top += tempDeltaY;
            t.bottom += tempDeltaY;
        }

        mapRc.left += tempDeltaX;
        mapRc.right += tempDeltaX;
        mapRc.top += tempDeltaY;
        mapRc.bottom += tempDeltaY;
    }*/

    turnManager->ProcessTurns(this);
    for (auto actor : actors)
    {
        if (actor) actor->Update();
    }
}

void Level::Render(HDC hdc)
{
    // PatBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);

    // RenderRect(hdc, mapRc);
    //sampleTile->DrawRect({(float)mapRc.left, (float)mapRc.top}, {(float)mapRc.right, (float)mapRc.bottom}, 1, 1);

    for (int i = 0; i < TILE_Y; ++i)
    {
        for (int j = 0; j < TILE_X; ++j)
        {
            if (!shouldBeRender[TILE_X * i + j]) continue;

            bool isVisible = map[TILE_X * i + j].visible;
            int tileType = map[TILE_X * i + j].type;
            int tileX = camera->ConvertToRendererX(tempTile[TILE_X * i + j].left);
            int tileY = camera->ConvertToRendererY(tempTile[TILE_X * i + j].top);

            // Get the current frame coordinates for this tile type
            POINT frame = GetCurrentFrame(tileType);

            // Render the tile using the frame coordinates
            sampleTile->RenderFrameScale(tileX, tileY, camera->GetZoomScale() * 2.f,
                                                camera->GetZoomScale() * 2.f, frame.x, frame.y);
            sampleTile->RenderFrameScale(tileX, tileY, camera->GetZoomScale() * 2.f,
                                    camera->GetZoomScale() * 2.f, 0, 9, 0, false, false, isVisible ? 0.f : 0.5f);
            // switch (map[TILE_X * i + j].type) {
            // 	case TT::WALL :
            // 		sampleTile->RenderFrameScale(
            // 			(camera->ConvertToRendererX(tempTile[TILE_X * i + j].left)),
            // 			(camera->ConvertToRendererY(tempTile[TILE_X * i + j].top)),
            // 			camera->GetZoomScale(), camera->GetZoomScale(), 1, 0);
            // 		// hOldBrush = (HBRUSH)SelectObject(hdc, GreyBrush);
            // 		// RenderRect(hdc, tempTile[20 * i + j]);
            // 		// SelectObject(hdc, hOldBrush);
            // 		break;
            // 	case TT::FLOOR:
            // 		sampleTile->RenderFrameScale(
            // 			(camera->ConvertToRendererX(tempTile[TILE_X * i + j].left)),
            // 			(camera->ConvertToRendererY(tempTile[TILE_X * i + j].top)),
            // 			camera->GetZoomScale(), camera->GetZoomScale(), 3, 0);
            // 		// hOldBrush = (HBRUSH)SelectObject(hdc, WhiteBrush);
            // 		// RenderRect(hdc, tempTile[20 * i + j]);
            // 		// SelectObject(hdc, hOldBrush);
            // 		break;
            // 	case TT::NONE:
            // 		sampleTile->RenderFrameScale(
            // 			(camera->ConvertToRendererX(tempTile[TILE_X * i + j].left)),
            // 			(camera->ConvertToRendererY(tempTile[TILE_X * i + j].top)),
            // 			camera->GetZoomScale(), camera->GetZoomScale(), 0, 0);
            // 		// hOldBrush = (HBRUSH)SelectObject(hdc, BlackBrush);
            // 		// RenderRect(hdc, tempTile[20 * i + j]);
            // 		// SelectObject(hdc, hOldBrush);
            // 		break;
            // 	default:
            // 		sampleTile->RenderFrameScale(
            // 			(camera->ConvertToRendererX(tempTile[TILE_X * i + j].left)),
            // 			(camera->ConvertToRendererY(tempTile[TILE_X * i + j].top)),
            // 			camera->GetZoomScale(), camera->GetZoomScale(), 1, 0);
            // 		// hOldBrush = (HBRUSH)SelectObject(hdc, RedBrush);
            // 		// RenderRect(hdc, tempTile[20 * i + j]);
            // 		// SelectObject(hdc, hOldBrush);
            // 		break;
            // }
        }
    }

    // sampleTile->DrawRect({(float)mapRc.left, (float)mapRc.top}, {(float)mapRc.right, (float)mapRc.bottom}, 1, 1);

    // for (int i = 0; i < TILE_Y; ++i)
    // {
    //     for (int j = 0; j < TILE_X; ++j)
    //     {
    //         int tileType = map[TILE_X * i + j].type;
    //         int tileX = static_cast<int>(tempTile[TILE_X * i + j].left);
    //         int tileY = static_cast<int>(tempTile[TILE_X * i + j].top);
    //         
    //         // Get the current frame coordinates for this tile type
    //         POINT frame = GetCurrentFrame(tileType);
    //         
    //         // Render the tile using the frame coordinates
    //         sampleTile->Middle_RenderFrameScale(tileX, tileY, 2.f, 2.f, frame.x, frame.y);
    //     }
    // }
    
    //Render Items
    for (auto item : items)
    {
        if (map[GetMapIndex(item->GetPosition().x, item->GetPosition().y)].visible)
        {
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
        if (map[GetMapIndex(actor->GetPosition().x, actor->GetPosition().y)].visible)
        {
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
    mapData = dungeonGenerator.Generate(width, height);

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
                GRID_POS_OFFSET.x + x * tempTileSize,
                GRID_POS_OFFSET.y + y * tempTileSize,
                GRID_POS_OFFSET.x + (x + 1) * tempTileSize,
                GRID_POS_OFFSET.y + (y + 1) * tempTileSize
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
            static_cast<float>(GRID_POS_OFFSET.x + tempTileSize),
            static_cast<float>(GRID_POS_OFFSET.y + tempTileSize)
        };
    }

    // 모든 바닥 타일 위치 수집
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            int tileType = mapData[y][x];
            // Check for any floor tile type (including variations)
            if (tileType == 1 || // TILE_FLOOR
                (tileType >= 20 && tileType <= 23)) // TILE_FLOOR_NORMAL to TILE_FLOOR_MOSSY
            {
                FPOINT pos;
                pos.x = static_cast<float>(GRID_POS_OFFSET.x + x * tempTileSize + tempTileSize / 2);
                pos.y = static_cast<float>(GRID_POS_OFFSET.y + y * tempTileSize + tempTileSize / 2);
                floorTiles.push_back(pos);
            }
        }
    }

    // If no floor tiles found, return a default position
    if (floorTiles.empty())
    {
        return {
            static_cast<float>(GRID_POS_OFFSET.x + tempTileSize),
            static_cast<float>(GRID_POS_OFFSET.y + tempTileSize)
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
    // // If we have frame data for this tile type
    // if (tileType >= 0 && tileType < frameMap.size() && !frameMap[tileType].empty())
    // {
    //     // Calculate current frame index based on time
    //     int frameIndex = static_cast<int>((frameTimer / FRAME_CHANGE_TIME) * frameMap[tileType].size()) % frameMap[tileType].size();
    //     return frameMap[tileType][frameIndex];
    // }

    // Default frames for basic tiles if no frame data exists
    switch (tileType)
    {
    case 0: // Wall
        return {0, 3};
    case 1: // Floor
        return {0, 0};
    case 2: // Door
        return {8, 3};
    case 3: // Entrance
        return {0, 1};
    case 4: // Exit
        return {1, 1};
    case 5: // Hidden door
        return {13, 3};
    case 6: // Empty space
        return {0, 9};
    case 10: // Top wall
        return {0, 6};
    case 11: // Bottom wall
        return {0, 12};
    case 12: // Left wall
        return {2, 9};
    case 13: // Right wall
        return {4, 9};
    case 14: // Top-left corner
        return {9, 6};
    case 15: // Top-right corner
        return {6, 3};
    case 16: // Bottom-left corner
        return {3, 13};
    case 17: // Bottom-right corner
        return {0, 13};
    default:
        return {0, 5}; // Default/unknown tile
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
                //map[i][j].SetColor(RGB(100, 100, 100));
                if (map[i * TILE_X + j].type == 0/* ||
                    map[i][j].GetType() == AstarTileType::Start ||
                    map[i][j].GetType() == AstarTileType::End*/)
                {
                    map[i * TILE_X + j].visible = false;
                    continue;
                }
                // map[i][j].SetColor(RGB(100, 100, 100));
            }
        }
    }
}

void Level::SetVisibleTile()
{
    //// 시야 설정-시야에 드는 타일은 색을 다르게 함.
    //// 시야 리셋
    ResetVisibleTile();

    // 임시 좌표-플레이어 좌표
    //map[10][10].SetColor(RGB(200, 200, 0));
    // map[10][10].isVisible = true;

    int index = GetMapIndex(player->GetPosition().x, player->GetPosition().y);
    int pTileXIndex = index % TILE_Y;
    int pTileYIndex = index / TILE_Y;
    map[index].visible = true;

    for (auto scanDirection : scanDirections)
    {
        fov->Calculate(reinterpret_cast<Map(&)[TILE_Y][TILE_X]>(map), pTileXIndex, pTileYIndex, 0,
                       1.0f, 0.0f, scanDirection);
        int a = 0;
    }
    // map[10][10].SetColor(RGB(200, 200, 0));
}
