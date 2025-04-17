#include "UIMopHPBar.h"
#include "UISlider.h"
#include "CommonFunction.h"

void UI::UIMopHPBar::Release()
{
}

void UI::UIMopHPBar::Update()
{
    hpBar->Update();
}

void UI::UIMopHPBar::Render(HDC hdc)
{
    hpBar->Render(hdc);
}

void UI::UIMopHPBar::SetPos(int dx, int dy)
{
    UIObject::SetPos(dx, dy);
    hpBar->SetPos(dx, dy);
}

void UI::UIMopHPBar::SetMaxHP(int hp)
{
    hpBar->SetMaxValue(hp);
}

void UI::UIMopHPBar::SetHP(int hp)
{
    hpBar->SetValue(hp);
}

void UI::UIMopHPBar::ResourceInit()
{
    hpBar = new UISlider();
    hpBar->Init(this, CaculateRelativeRECT(rectTransform, { 0, 0, 100, height}),
        ImageData{ "status_hp_bar2", L"assets/interfaces/HPBar.bmp", true, RGB(255, 255, 255) },
        ImageData{ "status_hp_bg2", L"assets/interfaces/BarBg.bmp", true, RGB(255, 255, 255) },
        ImageData{ "",L"",0,0 },
        { 3, 3, 3, 3 });
    hpBar->SetMaxValue(100);
    hpBar->SetValue(100);
}
