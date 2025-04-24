#include "TilemapTool.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "CommonFunction.h"
#include "Button.h"
#include "Tiles.h"
#include <functional>

HRESULT TilemapTool::Init()
{
	//SetClientRect(g_hWnd, TILEMAPTOOL_X, TILEMAPTOOL_Y);
	rtd = D2DImage::GetRenderTarget();
	//nowZoomScale = 0;

	sampleTile = D2DImageManager::GetInstance()->AddImage(
		"배틀시티_샘플타일", L"Image/tiles_sewers.png",
		16, 16);

	selectedTileType = 6;

	// 샘플 타일 영역
	/*tile000rc = { 900, 50, 948, 98 };
	tile110rc = { 1000, 50, 1048, 98 };
	tile111rc = { 1100, 50, 1148, 98 };*/
	sampleRc = { sampleOffset.x, sampleOffset.y, sampleOffset.x + gridSize * 12, sampleOffset.y + 2 * gridSize };

	sample_none = { sampleOffset.x, sampleOffset.y, sampleOffset.x + 2 * gridSize, sampleOffset.y + 2 * gridSize };
	sample_wall = { sample_none.right, sampleOffset.y, sample_none.right + 2 * gridSize, sample_none.top + 2 * gridSize };
	sample_floor = { sample_wall.right, sampleOffset.y, sample_wall.right + 2 * gridSize, sample_wall.top + 2 * gridSize };
	sample_door = { sample_floor.right, sampleOffset.y, sample_floor.right + 2 * gridSize, sampleOffset.y + 2 * gridSize };
	sample_entrance = { sample_door.right, sampleOffset.y, sample_door.right + 2 * gridSize, sampleOffset.y + 2 * gridSize };
	sample_destination = { sample_entrance.right, sampleOffset.y, sample_entrance.right + 2 * gridSize, sampleOffset.y + 2 * gridSize };

	
	for (auto& t : tileInfo)
	{
		t.type = selectedTileType;
		t.explored = true;
		t.visible = true;
	}
	

	if (wcslen(GameObject::nowFilePath) > 0) {
		Load();
	}

	/// 메인 타일 영역
	// whole box
	rcMain = { mainOffset.x, mainOffset.y, mainOffset.x + TILE_X * gridSize, mainOffset.y + TILE_Y * gridSize };
	// grid
	for (int i = 0; i < TILE_Y; ++i)
	{
		for (int j = 0; j < TILE_X; ++j)
		{
			mainGrid[i * TILE_X + j].left = mainOffset.x + j * gridSize;
			mainGrid[i * TILE_X + j].top = mainOffset.y + i * gridSize;
			mainGrid[i * TILE_X + j].right = mainGrid[i * TILE_X + j].left + gridSize;
			mainGrid[i * TILE_X + j].bottom = mainGrid[i * TILE_X + j].top + gridSize;
		}
	}
	
	// UI - 버튼
	saveButton = new Button();
	saveButton->Init(800,500);
	saveButton->SetFunction([this]() {
		this->Save();
		});

	gridColor = D2D1::ColorF(D2D1::ColorF::ForestGreen);
	selectedTileColor = D2D1::ColorF(D2D1::ColorF::Red);
	dragRcColor = D2D1::ColorF(D2D1::ColorF::Blue);

	rtd->CreateSolidColorBrush(gridColor, &rectBrush);

	dragRc = { 0, 0, 1, 1 };


	return S_OK;
}

void TilemapTool::Release()
{
	if (saveButton)
	{
		saveButton->Release();
		delete saveButton;
		saveButton = nullptr;
	}

	if (rectBrush) {
		rectBrush->Release();
	}
}

void TilemapTool::Update()
{
	POINT mouseP = MouseManager::GetInstance()->GetMousePos();

	if (PtInRect(&sample_none, mouseP))
	{
		if (MouseManager::GetInstance()->GetClickValueUsed() == false)
		{
			selectedTileType = 6;
			MouseManager::GetInstance()->InitPoints();
			MouseManager::GetInstance()->AlreadyClickUsed();
		}
	}

	if (PtInRect(&sample_wall, mouseP))
	{
		if (MouseManager::GetInstance()->GetClickValueUsed() == false)
		{
			selectedTileType = 0;
			MouseManager::GetInstance()->InitPoints();
			MouseManager::GetInstance()->AlreadyClickUsed();
		}
	}

	if (PtInRect(&sample_floor, mouseP))
	{
		if (MouseManager::GetInstance()->GetClickValueUsed() == false)
		{
			selectedTileType = 1;
			MouseManager::GetInstance()->InitPoints();
			MouseManager::GetInstance()->AlreadyClickUsed();
		}
	}

	if (PtInRect(&sample_door, mouseP))
	{
		if (MouseManager::GetInstance()->GetClickValueUsed() == false)
		{
			selectedTileType = 2;
			MouseManager::GetInstance()->InitPoints();
			MouseManager::GetInstance()->AlreadyClickUsed();
		}
	}

	if (PtInRect(&sample_entrance, mouseP))
	{
		if (MouseManager::GetInstance()->GetClickValueUsed() == false)
		{
			selectedTileType = 3;
			MouseManager::GetInstance()->InitPoints();
			MouseManager::GetInstance()->AlreadyClickUsed();
		}
	}

	if (PtInRect(&sample_destination, mouseP))
	{
		if (MouseManager::GetInstance()->GetClickValueUsed() == false)
		{
			selectedTileType = 4;
			MouseManager::GetInstance()->InitPoints();
			MouseManager::GetInstance()->AlreadyClickUsed();
		}
	}

	else if (PtInRect(&rcMain, mouseP))
	{
		if (MouseManager::GetInstance()->GetClickValueUsed() == false)
		{
			POINT clickP = MouseManager::GetInstance()->GetClickP();
			int posX = clickP.x;
			int posY = clickP.y;
			int tileX = (posX - mainOffset.x) / gridSize;
			int tileY = (posY - mainOffset.y) / gridSize;
			if (tileX >= 0 && tileX < TILE_X && tileY >= 0 && tileY < TILE_Y) {
				tileInfo[tileY * TILE_X + tileX].type = selectedTileType;

				MouseManager::GetInstance()->InitPoints();
				MouseManager::GetInstance()->AlreadyClickUsed();
			}
			
		}

		if (MouseManager::GetInstance()->GetIsDragging(MOUSE_LEFT)==true) {
			int posX = mouseP.x;
			int posY = mouseP.y;
			int tileX = (posX - mainOffset.x) / gridSize;
			int tileY = (posY - mainOffset.y) / gridSize;
			if (tileX >= 0 && tileX < TILE_X && tileY >= 0 && tileY < TILE_Y) {
				tileInfo[tileY * TILE_X + tileX].type = selectedTileType;
			}
		}

		if (MouseManager::GetInstance()->GetIsDragging(MOUSE_RIGHT) == true) {
			POINT dragStartP = MouseManager::GetInstance()->GetDragStartP(MOUSE_RIGHT);

			dragRc.left = min(dragStartP.x, mouseP.x);
			dragRc.right = max(dragStartP.x, mouseP.x);
			dragRc.top = min(dragStartP.y, mouseP.y);
			dragRc.bottom = max(dragStartP.y, mouseP.y);

		}

		if (MouseManager::GetInstance()->GetDragValueUsed(MOUSE_RIGHT) == false) {
			POINT dragStartP = MouseManager::GetInstance()->GetDragStartP(MOUSE_RIGHT);
			POINT dragEndP = MouseManager::GetInstance()->GetDragEndP(MOUSE_RIGHT);

			dragRc.left = min(dragStartP.x, dragEndP.x);
			dragRc.right = max(dragStartP.x, dragEndP.x);
			dragRc.top = min(dragStartP.y, dragEndP.y);
			dragRc.bottom = max(dragStartP.y, dragEndP.y);

			if (KeyManager::GetInstance()->IsStayKeyDown(VK_LSHIFT)) {

				MakeARoom();

			}
			else {
				for (int i = 0; i < TILE_Y; ++i) {
					for (int j = 0; j < TILE_X; ++j) {
						if (RectInRect(mainGrid[i * TILE_X + j], dragRc)) {
							tileInfo[i * TILE_X + j].type = selectedTileType;
						}
					}
				}
			}
			MouseManager::GetInstance()->InitPoints();
			MouseManager::GetInstance()->AlreadyDragUsed();

		}


	}

	if (saveButton)	saveButton->Update();

	if (KeyManager::GetInstance()->IsOnceKeyUp(VK_LBUTTON)) {
		AutoSave();
	}
}

void TilemapTool::Render(HDC hdc)
{
	//PatBlt(hdc, 0, 0, TILEMAPTOOL_X, TILEMAPTOOL_Y, WHITENESS);

	// 메인 타일 영역
	for (int i = 0; i < TILE_X * TILE_Y; ++i)
	{
		sampleTile->RenderFrameScale(mainGrid[i].left, mainGrid[i].top, 1.f,
			1.f, GetCurrentFrame(tileInfo[i].type).x, GetCurrentFrame(tileInfo[i].type).y);
	}

	if (gridLineOn) {

		if (rectBrush) {
			for (const auto& g : mainGrid) {
				// RECT → D2D1_RECT_F로 변환
				D2D1_RECT_F rect = D2D1::RectF(
					static_cast<float>(g.left),
					static_cast<float>(g.top),
					static_cast<float>(g.right),
					static_cast<float>(g.bottom)
				);

				// 선만 그리기 (채우기 없음)
				rectBrush->SetColor(gridColor);
				rtd->DrawRectangle(rect, rectBrush, 1.0f);
			}

		}
	}
	
	// 샘플 타일 영역
	sampleTile->RenderFrameScale(sample_none.left, sample_none.top, 2.f,
		2.f, GetCurrentFrame(6).x, GetCurrentFrame(6).y);

	sampleTile->RenderFrameScale(sample_wall.left, sample_wall.top, 2.f,
		2.f, GetCurrentFrame(0).x, GetCurrentFrame(0).y);

	sampleTile->RenderFrameScale(sample_floor.left, sample_floor.top, 2.f,
		2.f, GetCurrentFrame(1).x, GetCurrentFrame(1).y);

	sampleTile->RenderFrameScale(sample_door.left, sample_door.top, 2.f,
		2.f, GetCurrentFrame(2).x, GetCurrentFrame(2).y);

	sampleTile->RenderFrameScale(sample_entrance.left, sample_entrance.top, 2.f,
		2.f, GetCurrentFrame(3).x, GetCurrentFrame(3).y);

	sampleTile->RenderFrameScale(sample_destination.left, sample_destination.top, 2.f,
		2.f, GetCurrentFrame(4).x, GetCurrentFrame(4).y);

	if (rectBrush) {
		rectBrush->SetColor(selectedTileColor);
		D2D1_RECT_F rectToDraw = {};
		switch (selectedTileType) {
		case 6:
			rectToDraw = { (float)sample_none.left, (float)sample_none.top, (float)sample_none.right, (float)sample_none.bottom };
			break;
		case 0:
			rectToDraw = { (float)sample_wall.left, (float)sample_wall.top, (float)sample_wall.right, (float)sample_wall.bottom };
			break;
		case 1:
			rectToDraw = { (float)sample_floor.left, (float)sample_floor.top, (float)sample_floor.right, (float)sample_floor.bottom };
			break;
		case 2:
			rectToDraw = { (float)sample_door.left, (float)sample_door.top, (float)sample_door.right, (float)sample_door.bottom };
			break;
		case 3:
			rectToDraw = { (float)sample_entrance.left, (float)sample_entrance.top, (float)sample_entrance.right, (float)sample_entrance.bottom };
			break;
		case 4:
			rectToDraw = { (float)sample_destination.left, (float)sample_destination.top, (float)sample_destination.right, (float)sample_destination.bottom };
			break;
		}
		rtd->DrawRectangle(rectToDraw, rectBrush, 1.0f);

		if (MouseManager::GetInstance()->GetIsDragging(MOUSE_RIGHT) == true) {
			D2D1_RECT_F dragRect = { dragRc.left, dragRc.top, dragRc.right, dragRc.bottom };
			rectBrush->SetColor(dragRcColor);
			rtd->DrawRectangle(dragRect, rectBrush, 1.0f);
		}

	}

	if (saveButton) saveButton->Render(hdc);
}

void TilemapTool::AutoSave()
{
	if (GameObject::nowFilePath[0] != L'\0' && wcscmp(GameObject::nowFilePath, L"TestMapData.dat") != 0)
	{
		return;
	}
	// 파일 저장
	HANDLE hFile = CreateFile(
		L"TestMapData.dat", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, TEXT("파일 생성 실패"), TEXT("경고"), MB_OK);
		return;
	}
	DWORD dwByte = 0;
	WriteFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL);
	wcscpy_s(GameObject::nowFilePath, MAX_PATH, L"TestMapData.dat");
	CloseHandle(hFile);
}

void TilemapTool::Save()
{
	if (GameObject::nowFilePath[0] == L'\0' || wcscmp(GameObject::nowFilePath, L"TestMapData.dat") == 0)
	{
		SaveAs();
		return;
	}
	// save to exist file
	HANDLE hFile = CreateFile(
		nowFilePath, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, TEXT("파일 생성 실패"), TEXT("경고"), MB_OK);
		return;
	}
	DWORD dwByte = 0;
	WriteFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL);
	CloseHandle(hFile);
}

#include <commdlg.h>  // GetSaveFileName 사용을 위한 헤더

void TilemapTool::SaveAs()
{
	TCHAR prevDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, prevDir);
	OPENFILENAME ofn;
	WCHAR szFile[MAX_PATH] = L"TileMapData.dat"; 

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd; 
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"Tilemap 데이터 파일 (*.dat)\0*.dat\0모든 파일 (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_OVERWRITEPROMPT; 

	if (GetSaveFileName(&ofn))
	{
		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, TEXT("파일 생성 실패"), TEXT("경고"), MB_OK);
			return;
		}

		DWORD dwByte = 0;
		WriteFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL);
		wcscpy_s(GameObject::nowFilePath, MAX_PATH, ofn.lpstrFile);

		CloseHandle(hFile);
		SetCurrentDirectory(prevDir);
		MessageBox(g_hWnd, TEXT("저장 완료"), TEXT("알림"), MB_OK);
	}
}


void TilemapTool::Load()
{
	LPCWSTR pathToLoad = (GameObject::nowFilePath[0] != L'\0') ? GameObject::nowFilePath : L"TestMapData.dat";

	HANDLE hFile = CreateFile(
		pathToLoad, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, TEXT("파일 열기 실패"), TEXT("경고"), MB_OK);
		return;
	}

	DWORD dwByte = 0;
	if (!ReadFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL))
	{
		MessageBox(g_hWnd, TEXT("파일 읽기 실패"), TEXT("경고"), MB_OK);
	}
	CloseHandle(hFile);
}


#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


void TilemapTool::LoadAs()
{
	TCHAR prevDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, prevDir);
	// 파일 열기 다이얼로그 설정
	OPENFILENAME ofn;
	WCHAR szFile[MAX_PATH] = L""; // 초기 파일 이름은 없음

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd; // 네 윈도우 핸들
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"Tilemap 데이터 파일 (*.dat)\0*.dat\0모든 파일 (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST; // 존재하는 파일만 선택 가능

	// 사용자가 파일을 선택했을 경우
	if (GetOpenFileName(&ofn))
	{
		HANDLE hFile = CreateFile(
			ofn.lpstrFile, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(g_hWnd, TEXT("파일 열기 실패"), TEXT("경고"), MB_OK);
			return;
		}

		DWORD dwByte = 0;
		if (!ReadFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL))
		{
			MessageBox(g_hWnd, TEXT("파일 읽기 실패"), TEXT("경고"), MB_OK);
		}
		wcscpy_s(GameObject::nowFilePath, MAX_PATH, ofn.lpstrFile);
		CloseHandle(hFile);
		//MessageBox(g_hWnd, TEXT("불러오기 완료"), TEXT("알림"), MB_OK);
	}
	TCHAR loadedPath[MAX_PATH];
	SetCurrentDirectory(prevDir);
}

void TilemapTool::Erase()
{
	for (int i = 0; i < TILE_Y; ++i)
	{
		for (int j = 0; j < TILE_X; ++j)
		{
			tileInfo[i * TILE_X + j].type = 6;
			/*tileInfo[i * TILE_X + j].indX = j;
			tileInfo[i * TILE_X + j].indY = i;*/
		}
	}
}

void TilemapTool::Paint()
{
}

void TilemapTool::MakeARoom() /////// 타일 인덱스 예외처리
{
	for (int i = 0; i < TILE_Y; ++i) {
		for (int j = 0; j < TILE_X; ++j) {
			if (RectInRect(mainGrid[i * TILE_X + j], dragRc)) {
				tileInfo[i * TILE_X + j].type = 1;
				specialTiles.push_back({ j, i });
			}
		}
	}

	POINT dir[8] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1,1} };
	for (auto& ind : specialTiles) {
		for (int i = 0; i < 8; ++i) {
			if (tileInfo[(ind.y + dir[i].y) * TILE_X + (ind.x + dir[i].x)].type == 6) {
				tileInfo[(ind.y + dir[i].y) * TILE_X + (ind.x + dir[i].x)].type = 0;
			}
		}
	}

	specialTiles.clear();
}

void TilemapTool::Test()
{
	//SceneManager::GetInstance()->ChangeScene("전투씬_1");
}

POINT TilemapTool::GetCurrentFrame(int tileType)
{
	switch (tileType)
	{
	case 0: // Wall
		return { 0, 3 };
	case 1: // Floor
		return { 0, 0 };
	case 2: // Door
		return { 8, 3 };
	case 3: // Entrance
		return { 0, 1 };
	case 4: // Exit
		return { 1, 1 };
	case 5: // Hidden door
		return { 13, 3 };
	case 6: // Empty space
		return { 0, 9 };
	case 10: // Top wall
		return { 0, 6 };
	case 11: // Bottom wall
		return { 0, 12 };
	case 12: // Left wall
		return { 2, 9 };
	case 13: // Right wall
		return { 4, 9 };
	case 14: // Top-left corner
		return { 9, 6 };
	case 15: // Top-right corner
		return { 6, 3 };
	case 16: // Bottom-left corner
		return { 3, 13 };
	case 17: // Bottom-right corner
		return { 0, 13 };
	default:
		return { 0, 5 }; // Default/unknown tile
	}
}

