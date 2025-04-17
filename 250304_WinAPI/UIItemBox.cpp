#include "UIItemBox.h"
#include "UIText.h"
#include "UIIcon.h"
#include "ImageManager.h"
#include "CommonFunction.h"


using namespace UI;

void UIItemBox::Release()
{
	UIButton::Release();
	if (textUI2)
	{
		textUI2->Release();
		delete textUI2;
		textUI2 = nullptr;
	}
	if (itemIcon)
	{
		itemIcon->Release();
		delete itemIcon;
		itemIcon = nullptr;
	}
}

void UIItemBox::Render(HDC hdc)
{
	UIButton::Render(hdc);

	if (textUI2)
	{
		textUI2->Render(hdc);
	}

	if (itemIcon)
	{
		itemIcon->Render(hdc);
	}
}

void UIItemBox::SetText2(const string& str)
{
	if (textUI2)
	{
		textUI2->SetText(str);
	}
}

void UIItemBox::SetItem(ImageData itemImgData)
{
	itemIcon->Init(nullptr, rectTransform, itemImgData);
}

void UIItemBox::ResourceInit(ImageData imgData, ImageData bgData, RECT margin)
{
	UIButton::ResourceInit(imgData, bgData, margin);

	textUI->SetRect(CaculateRelativeRECT(rectTransform, { 0,0, width, height/2 }));
	textUI->SetText("TESTSTMI");

	textUI2 = new UIText;
	textUI2->Init(nullptr, CaculateRelativeRECT(rectTransform, { 0,height/2, width, height }), "ㄴㄹㄴㄹㄴ");

	itemIcon = new UIIcon;
}
