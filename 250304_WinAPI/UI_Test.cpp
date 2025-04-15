#include "UI_Test.h"
#include "Image.h"
#include "CommonFunction.h"
#include "config.h"
#include "UIStatus.h"

UI::UIStatus sta;

HRESULT UI_TestScene::Init()
{
	backGround = new Image();
	if (FAILED(backGround->Init(TEXT("Image/BackGround.bmp"), WINSIZE_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/backGround.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}

	sta.Init({ 0, 623, 335, 720 });

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
}

void UI_TestScene::Render(HDC hdc)
{
	backGround->Render(hdc);
	sta.Render(hdc);
}
