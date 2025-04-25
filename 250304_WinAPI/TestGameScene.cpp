#include "TestGameScene.h"
#include "Level.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "GameOver.h"

HRESULT TestGameScene::Init()
{

	testLevel = new TestLevel(nowFilePath);
	testLevel->Init(nullptr, 0, false);


	UIManager::GetInstance()->GetUiGameOver()->SetRestartCallBack([this]() {
		this->Restart(); });

	UIManager::GetInstance()->GetUiGameOver()->SetExitCallBack([this]() {
		this->Exit(); });
	
    return S_OK;
}

void TestGameScene::Release()
{
	if (testLevel) {
		testLevel->Release();
		delete testLevel;
		testLevel = nullptr;
	}
}

void TestGameScene::Update()
{
	if (testLevel) {
		testLevel->Update();
	}	
}

void TestGameScene::Render(HDC hdc)
{
	if (testLevel) {
		testLevel->Render(hdc);
	}
}

void TestGameScene::Restart()
{
	Release();
	testLevel = new TestLevel(nowFilePath);
	testLevel->Init(nullptr, 0, false);


	UIManager::GetInstance()->GetUiGameOver()->SetRestartCallBack([this]() {
		this->Restart(); });

	UIManager::GetInstance()->GetUiGameOver()->SetExitCallBack([this]() {
		this->Exit(); });
}

void TestGameScene::Exit()
{
	SceneManager::GetInstance()->RequestChangeScene("타일맵툴");
}

TestGameScene::TestGameScene()
{
}
