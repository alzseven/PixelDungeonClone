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

        void Init(RECT rect,
            ImageData imgSData, ImageData bgData = { "", L"", 0, 0 }, ImageData handleData = { "", L"", 0, 0 },
            RECT margin = {0,0,0,0});
        void Init(int dx,int dy, int width, int height, 
            ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, ImageData handleData = { "", L"", 0, 0 },
            RECT margin = { 0,0,0,0 });
        void Release() override;
        void Update() override;
        void Render(HDC hdc) override;

        virtual void SetMaxHP(float hp);
        virtual void SetHP(float hp);

    protected:
        virtual void ResourceInit(ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, ImageData handleData = { "", L"", 0, 0 },
            RECT margin = { 0,0,0,0 });
        float SmoothDamp(float current, float target, float& velocity, float smoothTime, float deltaTime);

    protected:
        Image* bg;
        Image* fill;
        Image* handleImg;

        RECT fillRectTransfrom{ 0,0,0,0 };
        int handleWidth;
        int handleHeight;

        float currentHP{ 100 };
        float goalHP{ 100 };
        float maxHP{ 100 };
        float fillValue = 1.0f;
        float elapsedTime = 0.0f;
        float timeDelta{ 0.0f };
        float currentVelocity = 0.0f;
        float smoothTime = 0.3f; // 값이 클수록 느리게, 작을수록 빠르게
    };
}