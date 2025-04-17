#pragma once
#include "config.h"
#include "UIObject.h"

class Image;

namespace UI
{

    class UISlider : public UIObject
    {
    public:
        UISlider() = default;
        ~UISlider() override = default;

        void Init(UIObject* parent, RECT rect,
            ImageData imgSData, ImageData bgData = { "", L"", 0, 0 }, ImageData handleData = { "", L"", 0, 0 },
            RECT margin = {0,0,0,0});
        void Init(UIObject* parent, int dx,int dy, int width, int height,
            ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, ImageData handleData = { "", L"", 0, 0 },
            RECT margin = { 0,0,0,0 });
        void Release() override;
        void Update() override;
        void Render(HDC hdc) override;

        void SetPos(int dx, int dy) override;
        virtual void SetMaxValue(float value);
        virtual void SetValue(float value);

    protected:
        virtual void ResourceInit(ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, ImageData handleData = { "", L"", 0, 0 },
            RECT margin = { 0,0,0,0 });
        virtual void ApplyFillImage();
        float SmoothDamp(float current, float target, float& velocity, float smoothTime, float deltaTime);

    protected:
        Image* bg;
        Image* fill;
        Image* handleImg;

        RECT margin{ 0,0,0,0 };
        RECT fillRectTransfrom{ 0,0,0,0 };
        float fillOriginWidth;
        float fillOriginHeight;
        int handleWidth;
        int handleHeight;

        float currentValue{ 100 };
        float goalValue{ 100 };
        float maxValue{ 100 };
        float fillValue = 1.0f;
        float elapsedTime = 0.0f;
        float timeDelta{ 0.0f };
        float currentVelocity = 0.0f;
        float smoothTime = 0.3f; // 값이 클수록 느리게, 작을수록 빠르게
    };
}