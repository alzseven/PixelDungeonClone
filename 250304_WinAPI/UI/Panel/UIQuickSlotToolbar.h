#pragma once
#include "../Core/UIContainer.h"
#include "../BUtton/UIImageTextButton.h"
#include "../Util/UIResourceSubManager.h"
#include "../../Observer/IEntityObserver.h"

class UIQuickSlotToolbar : public UIContainer, public IEntityObserver {
private:
    std::vector<UIImageTextButton*> itemSlots;
    std::vector<UIImageTextButton*> actionButtons;

public:
    void Init(const D2D1_RECT_F& rect = { 725.f, 648.f, 1080.f, 719.f }) {
        SetRect(rect);
        UIResourceSubManager::Preload_QuickSlotToolbar();

        AddItemSlots();
        AddActionButtons();
    }

    void SetActionOnClick(int index, std::function<void()> onClick) {
        if (index >= 0 && index < actionButtons.size()) {
             actionButtons[index]->SetOnClick(onClick);
        }
    }

private:
    void AddItemSlots() {
        const int slotCount = 3;
        const float slotWidth = 60.f;
        const float slotHeight = 71.f;

        UIIconStyle slotIconStyle =
        {
            { D2DImageManager::GetInstance()->FindImage("item_quick") }
        };
        
        for (int i = 0; i < slotCount; ++i) {
            auto* slot = new UIImageTextButton();
            slot->InitFromStyle(slotIconStyle,{ slotWidth * i, 5.f, slotWidth * (i + 1), slotHeight });
            itemSlots.push_back(slot);
            AddChild(slot);
        }
    }

    void AddActionButtons() {
        const char* keys[] = { "search_quick", "sleep_quick", "inven_quick" };
        const D2D1_RECT_F coords[] = {
            { 180.23f, 0.0f, 234.85f, 71.f },
            { 234.85f, 0.0f, 289.46f, 71.f },
            { 289.46f, 0.0f, 355.0f, 71.f }
        };

        UIIconStyle slotIconStyle =
        {
        { D2DImageManager::GetInstance()->FindImage("item_quick") },
        { D2DImageManager::GetInstance()->FindImage(keys[0]) }
        };
        auto* slot = new UIImageTextButton();
        slot->InitFromStyle(slotIconStyle, coords[0]);
        AddChild(slot);
        actionButtons.push_back(slot);


        slotIconStyle.iconStyle = { D2DImageManager::GetInstance()->FindImage(keys[1]) };
        slot = new UIImageTextButton();
        slot->InitFromStyle(slotIconStyle, coords[1]);
        AddChild(slot);
        actionButtons.push_back(slot);
        
        slotIconStyle.iconStyle = { D2DImageManager::GetInstance()->FindImage(keys[2]) };
        slot = new UIImageTextButton();
        slot->InitFromStyle(slotIconStyle, coords[2]);
        AddChild(slot);
        actionButtons.push_back(slot);
    }
};
