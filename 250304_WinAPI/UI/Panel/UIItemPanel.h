#pragma once
#include "../Text/UIText.h"
#include "../Button/UIImageTextButton.h"
#include "../Util/UIHelper.h"
#include "../UIData.h"
#include "../VisualStyle.h"
#include "D2DImageManager.h"
#include "../../Observer/IEntityObserver.h"

class UIItemPanel : public UIContainerBase, public IEntityObserver {
private:
    UIImageTextButton* itemIcon = nullptr;
    UIImageTextButton* itemName = nullptr;
    
    UIText* titleText = nullptr;
    UIImageTextButton* characterIcon = nullptr;
    UIText* infoTexts[6] = {};
    UIImageTextButton* tabButtons[3] = {};
    UIImageTextButton* infoButton = nullptr;

public:
    void Init(const D2D1_RECT_F& rect);
    void SetItemData(const PlayerStatData& data);
};

inline void UIItemPanel::Init(const D2D1_RECT_F& rect = { 400, 0, 650, 180 }) {
    SetRect(rect);

    NinePatchStyle bgNineStyle;
    bgNineStyle = UIHelper::CreateNinePatchFromSheet("chrome", "chrome", D2D1_SIZE_F {6.0f, 6.0f});
    auto bg = UIHelper::ApplyNinePathStyle(this, GetSizeRect(), bgNineStyle);

    // 아이콘
    characterIcon = UIHelper::ApplyIconStyle(this, {4, 4, 20, 20},
        UIIconStyle { {D2DImageManager::GetInstance()->FindImage("character_icon")}}
        );

    // 제목
    titleText = UIHelper::ApplyTextStyle(this, {20, 4, GetWidth(), 20},
        {L"pixel", 14.0f, D2D1::ColorF(D2D1::ColorF::Gold)});
    titleText->SetText(L"스탯창");

    // info 버튼
    infoButton = UIHelper::ApplyIconStyle(this, {this->GetWidth() - 24, 4, this->GetWidth() - 4, 24},
        {{D2DImageManager::GetInstance()->FindImage("info_icon")}});

    // 스탯 라벨 텍스트
    for (int i = 0; i < 6; ++i) {
        infoTexts[i] = UIHelper::ApplyTextStyle(this, {10, (28.0f + i * 16.0f), 220, 44},
            TextStyle{L"pixel", 13.0f, D2D1::ColorF(D2D1::ColorF::White)});
    }

    // 탭 버튼
    UIButtonStyle buttonStyle =
        {
            ImageStyle {D2DImageManager::GetInstance()->FindImage("inventory_bg")},
        TextStyle {L"pixel", 13.0f, D2D1::ColorF(D2D1::ColorF::White)},
        };
    NinePatchStyle btnNinePatchStyle = UIHelper::CreateNinePatchFromSheet("chrome", "chrome", D2D1_SIZE_F {6.0f, 6.0f});
    
    for (int i = 0; i < 3; ++i) {
        D2D1_RECT_F rect = {12 + i * 72.0f, 130, 12 + i * 72.0f + 60, 150};
        tabButtons[i] = UIHelper::ApplyNinePatchButtonStyle(this, rect,
            buttonStyle, btnNinePatchStyle);
        UIHelper::SetButtonText(*tabButtons[i], L"탭", 0);
        tabButtons[i]->SetOnClick([]()
        {
            
        });
    }
}

inline void UIItemPanel::SetStatData(const PlayerStatData& data) {
    if (titleText) titleText->SetText(data.title);

    if (infoTexts[0]) infoTexts[0]->SetText(L"힘 " + std::to_wstring(data.strength));
    if (infoTexts[1]) infoTexts[1]->SetText(L"체력 " + data.hp);
    if (infoTexts[2]) infoTexts[2]->SetText(L"경험치 " + data.exp);
    if (infoTexts[3]) infoTexts[3]->SetText(L"획득한 골드 " + std::to_wstring(data.gold));
    if (infoTexts[4]) infoTexts[4]->SetText(L"최대 도단 축수 " + std::to_wstring(data.maxFloor));
    if (infoTexts[5]) infoTexts[5]->SetText(L"던전 시드 " + data.dungeonSeed);
}
