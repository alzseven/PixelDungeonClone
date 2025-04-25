#pragma once
#include "UIEffectManager.h"

#include "Text/UITextEffect.h"
#include <vector>

#include "../Observer/IEntityObserver.h"
#include "../Camera.h"

UIEffectManager::~UIEffectManager()
{
    camera = nullptr;
    for (auto* fx : effects) {
        delete fx;
    }
    effects.clear();
};


void UIEffectManager::AddEffect(const std::wstring& text, const D2D1_RECT_F& rect)
{
    auto* fx = new UITextEffect();
    fx->SetWorldRect(rect);
    fx->Init(text, defaultTextStyle, defaultEffectStyle);
    effects.push_back(fx);
}

void UIEffectManager::AddEffect(const std::wstring& text, const TextStyle& style, const D2D1_RECT_F& rect, const EffectStyle& effectStyle) {
    auto* fx = new UITextEffect();
    fx->SetWorldRect(rect);
    fx->Init(text, style, effectStyle);
    effects.push_back(fx);
}

void UIEffectManager::Update(float dt) {
    for (auto it = effects.begin(); it != effects.end(); ) {
        if ((*it)->IsDead()) {
            delete *it;
            it = effects.erase(it);
        }
        else {
            (*it)->Update(dt);
            ++it;
        }
    }
}

void UIEffectManager::Render(ID2D1HwndRenderTarget* rt) {
    for (auto* fx : effects) {
        if (fx)
            fx->Render(rt);
    }
}

void UIEffectManager::Clear() {
    for (auto* fx : effects) {
        delete fx;
    }
    effects.clear();
}

void UIEffectManager::SetCamera(Camera* camera)
{
    if (camera)
    {
        this->camera = camera;
    }
}


/* Observer */
void UIEffectManager::OnEntityDamageTaken(DamageTakenData data)
{
    if (!camera)
    {
        OutputDebugStringA("카메라없어요~~~, by 이펙트매니저");
        return;
    }
    
    if (data.sendText == L"") return;

    if (!data.textStyle)
    {
        data.textStyle = &defaultTextStyle;
    }
    if (!data.effectStyle)
    {
        data.effectStyle = &defaultEffectStyle;
    }

    float width = 30.0f;
    float height = 30.0f;
    FPOINT scrPos = {camera->ConvertToRendererX(data.pos.x), camera->ConvertToRendererY(data.pos.y)};
    D2D1_RECT_F setRect = {scrPos.x + offset.width, scrPos.y + offset.height,
        scrPos.x + width + offset.width, scrPos.y + height + offset.height};
    
    AddEffect(data.sendText, *data.textStyle, setRect, *data.effectStyle);
}