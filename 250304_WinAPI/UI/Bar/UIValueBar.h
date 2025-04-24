#pragma once
#include "../Core/UIComponent.h"
#include "../VisualStyle.h"
#include "../Bar/ValueAnimator.h"
#include "../../D2DImageManager.h"

class UIValueBar : public UIComponent {
protected:
    BarStyle style;
    float maxValue = 100.0f;
    ValueAnimator animator;
    float fillPercent = 1.0f;

public:
    void Init(const D2D1_RECT_F& layout = {0,0,0,0},
        const BarStyle& s = {{nullptr},{nullptr},{nullptr}},
        float max = 100.0f);
    void SetStyle(const BarStyle& style);
    void SetValue(float value);
    void SetMaxValue(float max);

    void Update(float deltaTime) override;
    void Render(ID2D1HwndRenderTarget* renderTarget) override;
};

inline void UIValueBar::Init(const D2D1_RECT_F& layout, const BarStyle& s, float max) {
    SetRect(layout);
    SetStyle(s);
    SetMaxValue(max);
    style = s;
    maxValue = max;
    animator.SetInstant(max);
}

inline void UIValueBar::SetStyle(const BarStyle& style)
{
    this->style = style;
}

inline void UIValueBar::SetValue(float value) {
    animator.SetGoal(value);
}

inline void UIValueBar::SetMaxValue(float max) {
    maxValue = max;
    animator.SetGoal(max);
}

inline void UIValueBar::Update(float deltaTime) {
    float current = animator.Update(deltaTime);
    fillPercent = max(0.0f, min(current / maxValue, 1.0f));
}

inline void UIValueBar::Render(ID2D1HwndRenderTarget* rt) {
    D2D1_RECT_F rect = GetWorldRect();
    FPOINT ws = GetWorldScale();

    float width = rect.right - rect.left;
    float height = rect.bottom - rect.top;

    // 🔹 배경
    if (style.background.image)
        style.background.image->RenderFrameScale(rect.left, rect.top, ws.x, ws.y, 0, 0, 0.0f, false, false, style.background.alpha);

    // 🔹 fill (퍼센트 만큼만 채움)
    if (style.fill.image) {
        float fillWidth = width * fillPercent;
        style.fill.image->RenderPercent({ rect.left, rect.top }, 0, fillPercent * 100.f, style.fill.alpha);
    }

    // 🔹 handle (필요시)
    if (style.handle.image) {
        float handleX = rect.left + width * fillPercent - 4.0f; // 예시: 오른쪽 끝 - 약간 여백
        float handleY = rect.top;
        float handleWidth = 8.0f;
        float handleHeight = height;

        style.handle.image->RenderFrameScale(handleX, handleY, ws.x, ws.y, 0, 0, 0.0f, false, false, style.background.alpha);
    }
}
