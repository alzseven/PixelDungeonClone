#include "GameScene.h"
#include "CommonFunction.h"
#include "Level.h"
#include "UIManager.h"
#include "GameOver.h"

HRESULT GameScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);
	currLevel = testLevel;
	levels[testLevel] = new Level;
	if (levels[testLevel]) {
		levels[testLevel]->Init();
	}

	UIManager::GetInstance()->GetUiGameOver()->SetRestartCallBack([this]() {
		this->Restart();         // 함수 등록 (람다)
		});

	UIManager::GetInstance()->GetUiGameOver()->SetExitCallBack([]() {
		PostQuitMessage(0); });
	

	return S_OK;
}

void GameScene::Release()
{
	for (auto& l : levels) {
		if (l != nullptr) {
			l->Release();
			delete l;
			l = nullptr;
		}
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
	levels[testLevel] = new Level;
	if (levels[testLevel]) {
		levels[testLevel]->Init();
	}

	UIManager::GetInstance()->GetUiGameOver()->SetRestartCallBack([this]() {
		this->Restart(); });

	UIManager::GetInstance()->GetUiGameOver()->SetExitCallBack([]() {
		PostQuitMessage(0); });

}

GameScene::GameScene()
{
	for (auto& l : levels) {
		l = nullptr;
	}
}
