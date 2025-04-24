#pragma once

class D2DImage;

struct UIInventorySlotData
{
    //int itemId = 0;
    string name = "";
    D2DImage* icon = nullptr;
    int quantity = 0;
    int enhancement = 0;
};

struct PlayerStatData {
    std::wstring title{L"스탯창"};
    int strength = 0;
    std::wstring hp{L"10/20"};   
    std::wstring exp{L"3/10"};   
    int gold = 0;
    int maxFloor = 0;
    std::wstring dungeonSeed;
};