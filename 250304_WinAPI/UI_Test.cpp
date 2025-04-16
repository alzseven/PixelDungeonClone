#include "UI_Test.h"
#include "Image.h"
#include "CommonFunction.h"
#include "config.h"
#include "UIStatus.h"
#include "UIQuickSlot.h"
#include "UIMenueBtns.h"

UI::UIStatus sta;
UI::UIQuickSlot slot;
UI::UIMenueBtns menus;


HRESULT UI_TestScene::Init()
{
	backGround = new Image();
	if (FAILED(backGround->Init(TEXT("Image/BackGround.bmp"), WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/backGround.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}

	sta.Init({ 0, 623, 335, 720 });
	slot.Init({ 743,653, 337, 720 });
	menus.Init({ 920, 0, 1080, 56 });

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
	sta.Update();
	slot.Update();
	menus.Update();
}

void UI_TestScene::Render(HDC hdc)
{
	backGround->Render(hdc);
	sta.Render(hdc);
	slot.Render(hdc);
	menus.Render(hdc);
}
