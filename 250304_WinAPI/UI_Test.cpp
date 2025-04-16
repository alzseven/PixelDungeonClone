#include "UI_Test.h"
#include "Image.h"
#include "CommonFunction.h"
#include "config.h"
#include "UIView.h"

UI::UIView uiView;

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

	uiView.Init({ 0, 0, WINSIZE_X, WINSIZE_Y });

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
	uiView.Update();
}

void UI_TestScene::Render(HDC hdc)
{
	backGround->Render(hdc);
	uiView.Render(hdc);
}
