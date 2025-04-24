#pragma once
#include "../Core/UIComponent.h"
#include "../VisualStyle.h"
#include "../../DWriteFactory.h"

class UIText : public UIComponent {
private:
    std::wstring text;
    TextStyle style;

    IDWriteTextFormat* format = nullptr;
    ID2D1SolidColorBrush* brush = nullptr;
    IDWriteTextLayout* layout = nullptr;

    bool isAutoLayout = false;
public:
    ~UIText() override
    {
        if (format) { format->Release();format = nullptr; }
        if (brush) { brush->Release(); brush = nullptr; }
        if (layout) { layout->Release(); layout = nullptr; }

    }

    void Init(const TextStyle& s, const std::wstring& txt, const D2D1_RECT_F& rect, bool isAutoLayout = false) {
        this->isAutoLayout = isAutoLayout;

        SetRect(rect);

        text = txt;
        style = s;

        DWriteFactory::GetInstance()->CreateTextFormat(
            style.fontName,
            &format,
            style.fontSize,
            style.bold,
            style.horizontalAlign,
            style.verticalAlign
        );

        if (isAutoLayout)
        {
            DWriteFactory::GetInstance()->CreateTextLayout(
                text, format, GetWidth(), GetHeight(), &layout
            );
        }
    }

    void SetText(const std::wstring& txt, bool isAutoLayout = false) {
        this->isAutoLayout = isAutoLayout;
        
        text = txt;

        if (isAutoLayout)
        {
            DWriteFactory::GetInstance()->CreateTextLayout(
                text, format, GetWidth(), GetHeight(), &layout
            );

            ApplyTextMetricsToRect();
        }
    }

    void SetStyle(const TextStyle& s, bool isAutoLayout = false) {
        this->isAutoLayout = isAutoLayout;
        style = s;
        if (format) { format->Release(); format = nullptr; }

        DWriteFactory::GetInstance()->CreateTextFormat(
            style.fontName,
            &format,
            style.fontSize,
            style.bold,
            style.horizontalAlign,
            style.verticalAlign
        );

        if (isAutoLayout)
        {
            DWriteFactory::GetInstance()->CreateTextLayout(
                text, format, GetWidth(), GetHeight(), &layout
            );

            ApplyTextMetricsToRect();
        }

        if (brush) { brush->Release(); brush = nullptr; }
    }

    TextStyle& GetStyle() {
        return style;
    }
    void SetIsAutoLayout(bool isFlag)
    {
        this->isAutoLayout = isFlag;
    }

    void Render(ID2D1HwndRenderTarget* rt) override {
        if (!isVisible || !isActive) return;
        if (text.length() < 1) return;
        
        if (!brush && rt)
        {
            rt->CreateSolidColorBrush(style.color, &brush);
        }

        if (format && brush)
        {
            D2D1_RECT_F rect = GetScaledDrawRect();

            if (isAutoLayout)
            {
                rt->DrawTextLayout
                (
                    D2D1::Point2F(rect.left, rect.top),
                    layout,
                    brush
                );
            }
            else
            {
                rt->DrawTextW(text.c_str(), static_cast<UINT32>(text.length()), format, &rect, brush);
            }
            
            // 🔸 출력 영역 확인용 사각형 (얇은 외곽선)
            ID2D1SolidColorBrush* debugBrush = nullptr;
            rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &debugBrush);
            rt->DrawRectangle(rect, debugBrush, 1.0f);  // 1.0f: 선 두께

            if (debugBrush) debugBrush->Release();
            
        }


    }
    
    void ApplyTextMetricsToRect() {
        if (!layout) return;

        DWRITE_TEXT_METRICS metrics;
        if (SUCCEEDED(layout->GetMetrics(&metrics))) {
            // metrics.width / metrics.height 를 기준으로 컴포넌트 사이즈 갱신
            // 원래의 좌표값을 기준으로 size만 갱신
            auto rect = GetSizeRect(); // 원래 위치 유지
            rect.right = rect.left + metrics.width;
            rect.bottom = rect.top + metrics.height;
            SetRect(rect); // 이 함수가 내부적으로 localRect / worldRect 갱신하게 되어 있어야 함
        }
    }

};
