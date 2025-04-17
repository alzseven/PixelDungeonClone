#include "UIMenueBtns.h"
#include "UIObject.h"
#include "UIText.h"
#include "UIButton.h"
#include "CommonFunction.h"

using namespace UI;

void UIMenueBtns::Release()
{
	if (stairIcon)
	{
		stairIcon->Release();
		delete stairIcon;
		stairIcon = nullptr;
	}
	if (stairTextUI)
	{
		stairTextUI->Release();
		delete stairTextUI;
		stairTextUI = nullptr;
	}
	if (guideBookBtn)
	{
		guideBookBtn->Release();
		delete guideBookBtn;
		guideBookBtn = nullptr;
	}
	if (menuBtn)
	{
		menuBtn->Release();
		delete menuBtn;
		menuBtn = nullptr;
	}
}

void UIMenueBtns::Update()
{
}

void UIMenueBtns::Render(HDC hdc)
{
	if (stairIcon)
	{
		stairIcon->Render(hdc);
	}
	if (stairTextUI)
	{
		stairTextUI->Render(hdc);
	}
	if (guideBookBtn)
	{
		guideBookBtn->Render(hdc);
	}
	if (menuBtn)
	{
		menuBtn->Render(hdc);
	}
}

void UIMenueBtns::ResourceInit()
{
	stairIcon = new UIIcon();
	stairIcon->Init(this, CaculateRelativeRECT(rectTransform, {8,5,33,29}),
		ImageData{ "stair_ico", L"assets/interfaces/stair_ico.bmp", true, RGB(255, 255, 255) });
	stairTextUI = new UIText();
	stairTextUI->Init(this, CaculateRelativeRECT(rectTransform, {0,40,49,56}),
		"1", RGB(255, 255, 255));
	guideBookBtn = new UIButton();
	guideBookBtn->Init(this, CaculateRelativeRECT(rectTransform, {49, 0, 104, 56}), 
		ImageData{ "guidebook_btn", L"assets/interfaces/guidebook_btn.bmp", true, RGB(255, 255, 255) });
	menuBtn = new UIButton();
	menuBtn->Init(this, CaculateRelativeRECT(rectTransform, {104, 0, 159, 56}),
		ImageData{ "menu_btn", L"assets/interfaces/menu_btn.bmp", true, RGB(255, 255, 255) });

}
