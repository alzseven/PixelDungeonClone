#pragma once
#include "Text/UITextEffect.h"
#include <vector>

#include "../Observer/IEntityObserver.h"

class Camera;

class UIEffectManager : public IEntityObserver
{
private:
    Camera* camera{nullptr};
    
    std::vector<UITextEffect*> effects;
    D2D1_SIZE_F offset = {-15.f, -30.f};

    TextStyle defaultTextStyle = {
        L"pixel", 16.0f, D2D1::ColorF(D2D1::ColorF::White), true,
         DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER
    };

    EffectStyle defaultEffectStyle =
    {
        2.0f, // duration
        -20.0f // y movespeed value
    };

public:
    ~UIEffectManager() override;

    void AddEffect(const std::wstring& text, const D2D1_RECT_F& rect);
    void AddEffect(const std::wstring& text, const TextStyle& style,
        const D2D1_RECT_F& rect, const EffectStyle& effectStyle);
    void Update(float dt);
    void Render(ID2D1HwndRenderTarget* rt);
    void Clear();
    void SetCamera(Camera* camera);

    /* Observer */
    void OnEntityDamageTaken(DamageTakenData data) override;
};