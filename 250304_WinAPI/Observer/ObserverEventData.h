#pragma once
#include "../UI/VisualStyle.h"

class Entity;

struct DamageTakenData
{
    Entity* entity;
    std::wstring sendText;
    FPOINT pos{0,0};
    TextStyle* textStyle;
    EffectStyle* effectStyle;
};
