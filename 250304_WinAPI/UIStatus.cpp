#include "UIStatus.h"
#include "ImageManager.h"
#include "Image.h"
#include "UIIcon.h"
#include "UITextSlider.h"
#include "UITextBox.h"
#include "CommonFunction.h"
#include "string.h"

using namespace UI;

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
    if (levelTextUI)
    {
        levelTextUI->Release();
        delete levelTextUI;
        levelTextUI = nullptr;
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
	if (levelTextUI)
	{
		levelTextUI->Render(hdc);
	}
}

void UI::UIStatus::SetText(const string& text)
{
	if (levelTextUI)
	{
		levelTextUI->SetText(text);
	}
}

void UI::UIStatus::SetStatus(StatInfo statInfo)
{
    hpBar->SetMaxValue(statInfo.MaxHP);
    hpBar->SetValue(statInfo.HP);
    expBar->SetMaxValue(statInfo.MaxExp);
    expBar->SetValue(statInfo.Exp);
    levelTextUI->SetText("Lv. " + to_string(statInfo.Level));
}

void UIStatus::ResourceInit()  
{  
   auto imageManager = ImageManager::GetInstance();  

   icon = new UIIcon();  
   icon->Init(this, CaculateRelativeRECT(rectTransform, { 0, 0, 67, 68 }),  
       ImageData{ "status_character_ico", L"assets/sprites/TempCh.bmp", true, RGB(255, 255, 255) },  
       ImageData{ "status_character_bg", L"assets/sprites/TempChBg.bmp", true, RGB(255, 255, 255) }  
       , { 10, 10 , 0, 0 });  
   hpBar = new UITextSlider();  
   hpBar->Init(this, CaculateRelativeRECT(rectTransform, { 67, 34, 355, 68 }),  
       ImageData{ "status_hp_bar", L"assets/interfaces/HPBar.bmp", true, RGB(255, 255, 255) },  
       ImageData{ "status_hp_bg", L"assets/interfaces/BarBg.bmp", true, RGB(255, 255, 255) }, 
       ImageData{"",L"",0,0},
       {5, 9, 10, 5});
   hpBar->SetMaxValue(100);
   hpBar->SetValue(70);

   expBar = new UITextSlider();  
   expBar->Init(this, CaculateRelativeRECT(rectTransform, { 67, 68, 355, 97 }),  
       ImageData{ "status_exp_bar", L"assets/interfaces/ExpBar.bmp", true, RGB(255, 255, 255) },  
       ImageData{ "status_exp_bg", L"assets/interfaces/BarBg.bmp", true, RGB(255, 255, 255) },
       ImageData{ "",L"",0,0 },
       { 5, 7, 10, 5 });
   expBar->SetMaxValue(30);
   expBar->SetValue(13);

   levelTextUI = new UITextBox();  
   levelTextUI->Init(this, CaculateRelativeRECT(rectTransform, { 0, 68, 67, 97 }), "lv. 1",
       ImageData{ "status_level", L"assets/interfaces/level_box.bmp", true, RGB(255,255,255) }
   , { 0, 5, 0, 0 });
}
