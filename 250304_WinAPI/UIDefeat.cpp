#include "UIDefeat.h"
#include "ImageManager.h"
#include "UIImage.h"
#include "CommonFunction.h"

void UI::UIDefeat::Release()
{
    if (defeatImage)
    {
        defeatImage->Release();
        delete defeatImage;
        defeatImage = nullptr;
    }
}

void UI::UIDefeat::Update()
{
    defeatImage->Update();
}

void UI::UIDefeat::Render(HDC hdc)
{
    if (defeatImage)
    {
        defeatImage->Render(hdc);
    }
}

void UI::UIDefeat::ResourceInit()
{
    defeatImage = new UIImage();
    defeatImage->Init(CaculateRelativeRECT(rectTransform, { 0, 0, width/3, height/5}),
        ImageData{ "defeat_img", L"assets/interfaces/defeat_img.bmp", true, RGB(255, 255, 255) },
        { 0,  0, 0, 0 });
    defeatImage->SetPos(centerX, height / 3);
}
