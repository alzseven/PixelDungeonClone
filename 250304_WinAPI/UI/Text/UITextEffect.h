#pragma once
#include "../Core/UIContainerBase.h"
#include "../VisualStyle.h"
#include "../Text/UIText.h"

class UITextEffect : public UIContainerBase {
private:
    TextStyle textStyle;
    EffectStyle effectStyle;
    UIText* text = nullptr;
    float elapsed = 0.0f;    // 경과 시간
    float startY = 0.0f;

public:
    void Init(const std::wstring& str, const TextStyle& textStyle, EffectStyle effectStyle) {
        this->textStyle = textStyle;
        this->effectStyle = effectStyle;
        elapsed = 0.0f;
        startY = worldRect.top;

        text = new UIText();
        text->Init(textStyle, str, {0, 0, localRect.right, localRect.bottom});
        AddChild(text);
    }

    void SetTextStyle(const TextStyle& textStyle)
    {
        this->textStyle = textStyle;
    }
    void SetEffectStyle(const EffectStyle& effectStyle)
    {
        this->effectStyle = effectStyle;
    }

    void Update(float dt) override {
        if (!isActive || IsDead() || !text) return;

        elapsed += dt;

        // 알파값 줄이기
        float t = min(elapsed / effectStyle.lifetime, 1.0f);
        float fade = 1.0f - std::pow(2.0f, 10.0f * (t - 1.0f));
        TextStyle s = text->GetStyle();
        s.color.a = fade;
        text->SetStyle(s);

        // 기준 위치 + offset 계산
        float offset = effectStyle.moveSpeed * elapsed;
        D2D1_RECT_F newRect = {
            GetWorldRect().left,
            GetWorldRect().top + offset,
            GetWorldRect().right,
            GetWorldRect().bottom + offset
        };
        text->SetWorldRect(newRect);
    }

    void Render(ID2D1HwndRenderTarget* rt) override {
        if (!isVisible || !isActive) return;
        
        if (!IsDead() && text) {
            text->Render(rt);
        }
    }

    bool IsDead() const { return elapsed >= effectStyle.lifetime; }
};
