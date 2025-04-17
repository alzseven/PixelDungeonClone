#include "UI_Test.h"
#include "Image.h"
#include "CommonFunction.h"
#include "config.h"
#include "UIManager.h"
#include "UIMopHPBar.h"
UI::UIMopHPBar mopHPBar;

UI::UIManager UIManager;

HRESULT UI_TestScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	backGround = new Image();
	if (FAILED(backGround->Init(TEXT("Image/BackGround.bmp"), WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/backGround.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}

	UIManager.Init();
	mopHPBar.Init(nullptr, WINSIZE_X/2, WINSIZE_Y/2, 50, 10);
	mopHPBar.SetMaxHP(30);
	mopHPBar.SetHP(10);

    return S_OK;
}

void UI_TestScene::Release()
{
	if (backGround)
	{
		backGround->Release();
		delete backGround;
		backGround = nullptr;
	}
}

void UI_TestScene::Update()
{
	UIManager.Update();
	mopHPBar.Update();
	mopHPBar.SetPos(g_ptMouse.x, g_ptMouse.y);

	if (KeyManager::GetInstance()->IsOnceKeyDown('K'))
	{
		UIManager.ShowDefeat(true);
		UIManager.SetStatus(UI::StatInfo{ 100, 100, 100, 100, 100 });
		mopHPBar.SetHP(20);
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('R'))
	{
		UIManager.Reset();
		mopHPBar.SetHP(10);
	}
}

void UI_TestScene::Render(HDC hdc)
{
	backGround->Render(hdc);
	UIManager.Render(hdc);
	mopHPBar.Render(hdc);
}
