#pragma once
#include "config.h"
#include "DungeonGenerator.h"
#include "IntegratedDungeonSystem.h"
#include "Monster.h"

class Player;
class FieldOfView;
class Monster;
class D2DImage;
class TurnManager;
class Entity;
class Item;
class Biome;
class Image;
class Camera;
class UIManager;
class Level
{
private:

	//map
	Map map[TILE_Y * TILE_X];
	RECT mapRc;
	RECT tempTile[TILE_Y * TILE_X]; //타일 이미지 넣기 전 임시 이미지 그리기용 배열
	int tempTileSize; // 20*20 규격의 맵에 알맞은 임시 타일 사이즈

	Camera* camera;

	// Frame map data for tile animations/variants
	// std::vector<std::vector<POINT>> frameMap;  // Stores frame coordinates for each tile type
	float frameTimer;                          // Timer for frame animation
	const float FRAME_CHANGE_TIME = 0.2f;      // Time between frame changes

	float nowZoomScale;

	bool shouldBeRender[TILE_Y * TILE_X]; //인덱스별 렌더해야하는지 여부 저장하는 배열
	bool hasExplored[TILE_Y * TILE_X]; //인덱스별 해금되었는지 여부 저장하는 배열
	bool isSeen[TILE_Y * TILE_X]; //인덱스별 시야 안에 들어왔는지 여부 저장하는 배열

	// const POINT GRID_POS_OFFSET = {(3240 - TILE_SIZE*TILE_X)/2, (2160 - TILE_SIZE * TILE_Y) / 2 };
	const POINT GRID_POS_OFFSET = {0,0};
	vector<Biome*> bioms;
	vector<Item*> items;
	vector<Entity*> monsters;

	D2DImage* sampleTile;

	std::vector<std::vector<int>> mapData;
    
	// 던전 생성기
	DungeonGenerator dungeonGenerator;
    
	// 맵 크기
	int mapWidth;
	int mapHeight;
	
	// HBRUSH BlackBrush;
	// HBRUSH GreyBrush;
	// HBRUSH WhiteBrush;
	// HBRUSH RedBrush;
	//
	// HBRUSH hOldBrush;

	// Entity* player;
	TurnManager* turnManager;

	Player* player;
	vector<Entity*> actors;
	// int startInd;
	IntegratedDungeonSystem dungeonSystem;

	// Helper method to get current frame for a tile type
	POINT GetCurrentFrame(int tileType) const;

	// FoV
	FieldOfView* fov;
	// config
	ScanDirection scanDirections[8] =
	{
		{1, 0, 0, -1},
		{-1, 0, 0, -1},
		{0, 1, 1, 0},
		{0, -1, 1, 0},
		{-1, 0, 0, 1},
		{1, 0, 0, 1},
		{0, -1, -1, 0},
		{0, 1, -1, 0}
	};

	// UI Sample
	UIManager* uiManager;;
	
public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void AddActor(Entity* actor);
	void AddItem(Item* item);
	int GetMapIndex(int x, int y) {return (y - GRID_POS_OFFSET.y - TILE_SIZE / 2) / TILE_SIZE * TILE_X + (x - GRID_POS_OFFSET.x - TILE_SIZE / 2) / TILE_SIZE;}
	Map* GetMap(int x, int y) { return &map[(y - GRID_POS_OFFSET.y - TILE_SIZE / 2) / TILE_SIZE * TILE_X + (x - GRID_POS_OFFSET.x - TILE_SIZE / 2) / TILE_SIZE]; }
	Entity* GetActorAt(FPOINT pos);
	Item* GetItemAt(FPOINT pos);
	void MoveItemToInven(Item* item);

	FPOINT GetPosByGridIndex(int idx, int idy) const { return {GRID_POS_OFFSET.x + (idx * 2 + 1) / 2.f * TILE_SIZE, GRID_POS_OFFSET.y + (idy * 2 + 1) / 2.f * TILE_SIZE }; }
	
	void FileLoad();

	void GenerateMap(int width, int height);
    
	// 맵 정보 조회 메서드
	int GetTileType(int x, int y) const;
	bool IsSolid(int x, int y) const;
	FPOINT GetRandomFloorTile() const;
	FPOINT GetEntranceSpawnPosition() const;
	void ResetVisibleTile();
	void SetVisibleTile();

	Level();
	~Level();

	void SetMapData(const std::vector<std::vector<int>>& mapData)
	{
		this->mapData = mapData;
		int i = 0;
		for (const auto& vec : mapData)
		{
			for (auto m : vec)
			{
				this->map[i] = Map(m);
				i++;
			}
		}
		
	}

	void AddMonsters(const std::vector<Monster*>& monsters)
	{
		for (Entity* monster : monsters) {
			actors.push_back(static_cast<Entity*>(monster));
		}
	}
	std::vector<std::vector<int>> GetMapData() const
	{
		return mapData;
	}
	void SetTile(int x, int y, int tileType)
	{
		map[y * TILE_X + x].type = tileType;
	}
	
};

