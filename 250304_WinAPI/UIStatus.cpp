#include "UIStatus.h"
#include "ImageManager.h"
#include "Image.h"
#include "UIIcon.h"
#include "UISlider.h"
#include "UIText.h"
#include "CommonFunction.h"

using namespace UI;

void UIStatus::Init(RECT rect)
{
    UIObject::Init(rect);

    ResourceInit();
}

void UIStatus::Init(int dx, int dy, int width, int height)
{
    UIObject::Init(dx, dy, width, height);

    ResourceInit();
}

void UIStatus::Release()
{
    if (icon)
    {
        icon->Release();
		delete icon;
        icon = nullptr;
    }

    if (hpBar)
    {
        hpBar->Release();
        delete hpBar;
        hpBar = nullptr;
    }

	if (expBar)
	{
		expBar->Release();
		delete expBar;
		expBar = nullptr;
	}
    if (levelText)
    {
        levelText->Release();
        delete levelText;
        levelText = nullptr;
    }
}

void UIStatus::Update()
{

	if (hpBar)
	{
		hpBar->Update();
	}
	if (expBar)
	{
		expBar->Update();
	}
	if (levelText)
	{
		levelText->Update();
	}
}

void UIStatus::Render(HDC hdc)
{
    if (icon)
    {
        icon->Render(hdc);
    }
	if (hpBar)
	{
		hpBar->Render(hdc);
	}
	if (expBar)
	{
		expBar->Render(hdc);
	}
	if (levelText)
	{
		levelText->Render(hdc);
	}
}

void UI::UIStatus::ResourceInit()
{
    auto imageManager = ImageManager::GetInstance();

    icon = new UIIcon();
    icon->Init(CaculateRelativeRECT(rectTransform, { 0, 0, 67, 68 }),
        ImageData{ "status_character_ico", L"assets/sprites/TempCh.bmp", true, RGB(255, 255, 255) },
        ImageData{ "status_character_bg", L"assets/sprite/TempChBg.bmp", true, RGB(255, 255, 255) });
	hpBar = new UISlider();
	hpBar->Init(CaculateRelativeRECT(rectTransform, { 67, 34, 355, 68 }),
		ImageData{ "status_hp_bar", L"assets/interfaces/HPBar.bmp", true, RGB(255, 255, 255) },
		ImageData{ "status_hp_bg", L"assets/interfaces/HPBarBg.bmp", true, RGB(255, 255, 255) });
	expBar = new UISlider();
	expBar->Init(CaculateRelativeRECT(rectTransform, { 67, 68, 355, 97 }),
		ImageData{ "status_exp_bar", L"assets/interfaces/ExpBar.bmp", true, RGB(255, 255, 255) },
		ImageData{ "status_exp_bg", L"assets/interfaces/HPBarBg.bmp", true, RGB(255, 255, 255) });

 //   UIText* levelText;
}
