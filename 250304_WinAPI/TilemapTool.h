#pragma once
#include "GameObject.h"


class D2DImage;
class Button;
class ExitButton;
class TilemapTool : public GameObject
{
private:
	ID2D1HwndRenderTarget* rtd;
	D2DImage* sampleTile;
	//D2DImage* selectedTile;
	D2DImage* grid;

	//const float zoomScales[5] = { 1.0f, 1.5f, 2.0f, 2.5f, 3.0f };
	//int nowZoomScale;

	Map tileInfo[TILE_X * TILE_Y];

	ExitButton* GoLobbyButton;

	const int gridSize{ 16 };
	D2D1_COLOR_F gridColor;
	D2D1_COLOR_F selectedTileColor;
	D2D1_COLOR_F dragRcColor;

	ID2D1SolidColorBrush* rectBrush{ nullptr };
	
	//POINT selectedTile;

	Button* saveButton;

	int selectedTileType;
	const POINT sampleOffset = { 760, 40 };
	RECT sampleRc;

	RECT sample_none;
	RECT sample_wall;
	RECT sample_floor;
	RECT sample_door;
	RECT sample_entrance;
	RECT sample_destination;

	RECT dragRc;
	vector<POINT> specialTiles;
	//POINT dragStartP;
	//POINT dragNowP;

	//bool isDragging;
	const POINT mainOffset = { 10, 40 };
	RECT rcMain;
	RECT mainGrid[TILE_X * TILE_Y];

	bool gridLineOn{ true };

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void AutoSave();
	void Save();
	void SaveAs();
	void Load();
	void LoadAs();

	void Erase();
	void Paint();
	void Exit();

	void MakeARoom();

	inline void SetGridLine() { gridLineOn = !gridLineOn; }
	void Test();

	POINT GetCurrentFrame(int tileType);

	TilemapTool() {};
	virtual ~TilemapTool() {};
};

