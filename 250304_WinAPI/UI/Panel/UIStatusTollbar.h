#pragma once
#include "../Core/UIContainer.h"
#include "../Bar/UIValueBar.h"
#include "../Button/UIImageTextButton.h"
#include "../Text/UITextBox.h"
#include "../Util/UIResourceSubManager.h"
#include "../../Observer/IEntityObserver.h"

class UIStatusToolbar : public UIContainer, public IEntityObserver {
private:
    UIValueBar* hpBar = nullptr;
    UIValueBar* expBar = nullptr;
    UITextBox* levelText = nullptr;

public:
    void OnEntityDamageTaken(DamageTakenData data) override
    {
        SetHP(data.entity->GetHP(), data.entity->GetMaxHP());
    }
    
    void Init(const D2D1_RECT_F& rect = { 0.0f, 610.0f, 398.0f, 711.0f }) override
    {
        SetRect(rect);
        UIResourceSubManager::Preload_StatusToolbar();

        AddHPBar();
        AddEXPBar();
        AddCharacterIcon();
        AddLevelText();
    }

    void SetHP(int current, int max) {
        if (hpBar) {
            hpBar->SetMaxValue((int)max);
            hpBar->SetValue((int)current);
        }
    }

    void SetEXP(int current, int max) {
        if (expBar) {
            expBar->SetMaxValue((int)max);
            expBar->SetValue((int)current);
        }
    }

    void SetLevel(int level) {
        if (levelText) {
            levelText->SetText(L"Lv. " + std::to_wstring(level));
        }
    }

private:
    void AddHPBar() {
        hpBar = new UIValueBar();
        hpBar->Init({ 75, 37, 398.5f, 105.13f }, BarStyle {
            { D2DImageManager::GetInstance()->FindImage("status_hp_bg"), 1.0f },
            { D2DImageManager::GetInstance()->FindImage("status_hp_bar"), 1.0f }
            }, 100);
        AddChild(hpBar);
    }

    void AddEXPBar() {
        expBar = new UIValueBar();
        expBar->Init({ 73, 69, 396.5f, 101.13f }, BarStyle{
            { D2DImageManager::GetInstance()->FindImage("status_exp_bg"), 1.0f },
            { D2DImageManager::GetInstance()->FindImage("status_exp_bar"), 1.0f }
            }, 30);
        AddChild(expBar);
    }

    void AddCharacterIcon() {
        auto* icon = new UIImageTextButton();
        icon->Init({ 0, 0, 74.25f, 74.25f });
        AddChild(icon);

        UIIconStyle charIconStyle =
        {
 {D2DImageManager::GetInstance()->FindImage("status_character_bg"), },
{D2DImageManager::GetInstance()->FindImage("status_character_ico"), { 17.32f, 9.0f, 17.32f, 9.0f }},
        };
            
        icon->InitFromStyle(charIconStyle, {0,0, icon->GetWidth(), icon->GetHeight()});
    }

    void AddLevelText() {

        UIButtonStyle style = {
            { D2DImageManager::GetInstance()->FindImage("status_level") },
            { L"pixel", 16.0f, D2D1::ColorF(D2D1::ColorF::White), true,
              DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER }
        };
        levelText = new UITextBox();
        levelText->Init(style, L"Lv. 1", { 0, 74, 74.25f, 98.75f });
        AddChild(levelText);
    }
};
