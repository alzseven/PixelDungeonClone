#include "GameScene.h"
#include "CommonFunction.h"
#include "Level.h"
#include "UIManager.h"
#include "GameOver.h"
#include "Player.h"
HRESULT GameScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	currLevel = testLevel;
	LevelInit(currLevel);
	
	logColor = { 1.0f, 0.4f, 0.4f, 1.0f };



	player = new Player({0,0}, 1000.f, 20, 50, 2);
	player->SetFunctions([this]() {this->Ascend(); }, [this]() {this->Descend(); });
	uiManager = UIManager::GetInstance();
	uiManager->Init();
	uiManager->RegisterPlayer(player);
	uiManager->GetUiGameOver()->SetRestartCallBack([this]() {
		this->Restart();         // 함수 등록 (람다)
		});

	uiManager->GetUiGameOver()->SetExitCallBack([]() {
		PostQuitMessage(0); });


	// 시스템 로그-던전 입장
	wstring floor = to_wstring(testLevel);
	// 한글
	wstring kor = L"던전의 " + floor  + L"층으로 내려왔다.";
	UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::Yellow));
	// 영어
	//wstring eng = L"Welcome to the level " + floor + L" of Pixel Dungeon!";
	//UIManager::GetInstance()->SendLog(eng, D2D1::ColorF(D2D1::ColorF::White));
	
	
	/*for (int i = 0; i < 7; ++i) {
		levels[i] = new Level();
		levels[i]->Init(player, i+1, true);
	}*/
	levels[0] = new Level();
	levels[0]->Init(player, 1, true);
	for (int i = 1; i < 6; ++i) {
		levels[i] = nullptr;
	}

	player->SetPosition(levels[0]->GetPlayerInitP());
	player->SetStairs(levels[0]->GetAscPos(), levels[0]->GetDescPos());


	return S_OK;
}

void GameScene::Release()
{
	uiManager->DeleteLevelUI();
	uiManager = nullptr;

	for (auto& l : levels) {
		if (l != nullptr) {
			l->Release();
			delete l;
			l = nullptr;
		}
	}

	if (player)
	{
		//delete player;
		player = nullptr;
		UIManager::GetInstance()->SetCurrentPlayer(nullptr);
	}
}

void GameScene::Update()
{
	levels[currLevel]->Update();
}

void GameScene::Render(HDC hdc)
{
	levels[currLevel]->Render(hdc);
}

void GameScene::Restart()
{
	for (auto& l : levels) {
		if (l != nullptr)
		{
			l->Release();
			delete l;
			l = nullptr;
		}
	}

	currLevel = testLevel;
	LevelInit(currLevel);
	
	Release();
	Init();

	UIManager::GetInstance()->GetUiGameOver()->SetRestartCallBack([this]() {
		this->Restart(); UIManager::GetInstance()->SendLog(L"Click", D2D1::ColorF(D2D1::ColorF::Blue)); });
}

void GameScene::Ascend()
{
	currLevel--;
	if (currLevel < 0) {
		currLevel = 0;
		// 로그창에 현재층 0층이라는 거 나오게 하기
		uiManager->SendLog(L"현재 계신 곳이 최상층입니다.", logColor);
		player->SetJustMoved(true);
		return;
	}
	else {
		if (levels[currLevel] == nullptr) return;
		player->SetPosition(levels[currLevel]->GetDescPos());
		player->SetNextPos(levels[currLevel]->GetDescPos());
		player->SetStairs(levels[currLevel]->GetAscPos(), levels[currLevel]->GetDescPos());
		player->SetJustMoved(true);
	}
	
}

void GameScene::Descend()
{
	currLevel++;
	if (currLevel > 6) {
		currLevel = 6;
		// 로그창에 현재층 마지막 층이라는 거 나오게 하기
		uiManager->SendLog(L"현재 계신 곳이 마지막 층입니다.", logColor);
		player->SetJustMoved(true);
		return;
	}
	else {
		levels[currLevel] = new Level();
		levels[currLevel]->Init(player, currLevel + 1, true);

		player->SetPosition(levels[currLevel]->GetPlayerInitP());
		player->SetNextPos(levels[currLevel]->GetPlayerInitP());
		player->SetStairs(levels[currLevel]->GetAscPos(), levels[currLevel]->GetDescPos());
		player->SetJustMoved(true);
	}
}

GameScene::GameScene()
{
	for (auto& l : levels) {
		l = nullptr;
	}
}

void GameScene::LevelInit(int testLevel)
{
	levels[testLevel] = new Level;
	if (levels[testLevel]) {
		levels[testLevel]->Init();
		UIManager::GetInstance()->SendLog(
		L"Wellcom To " + to_wstring(testLevel + 1) + L" of Pixel Dungeon!",
		D2D1::ColorF(D2D1::ColorF::White));
		wstring kor = L"던전의 " + to_wstring(testLevel + 1) + L"층으로 내려왔다.";
		UIManager::GetInstance()->SendLog(kor, D2D1::ColorF(D2D1::ColorF::Yellow));
	}
}
