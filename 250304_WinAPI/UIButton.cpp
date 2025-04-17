#include "UIButton.h"
#include "UIText.h"

using namespace UI;

void UIButton::Init(UIObject* parent, RECT rect, ImageData imgData, ImageData bgData, RECT margin)
{
	UIIcon::Init(parent, rect, imgData, bgData, margin);
}

void UIButton::Init(UIObject* parent, int dx, int dy, int width, int height, ImageData imgData, ImageData bgData, RECT margin)
{
	UIIcon::Init(parent, dx, dy, width, height, imgData, bgData, margin);
}

void UIButton::Release()
{
	if (textUI)
	{
		textUI->Release();
		delete textUI;
		textUI = nullptr;
	}

	UIIcon::Release();
}

void UIButton::Update()
{
}

void UIButton::Render(HDC hdc)
{
	UIIcon::Render(hdc);
	if (textUI)
	{
		textUI->Render(hdc);
	}
}

void UIButton::SetText(string& txt)
{
	if (textUI)
	{
		textUI->SetText(txt);
	}
}

void UIButton::ResourceInit(ImageData imgData, ImageData bgData, RECT margin)
{
	UIIcon::ResourceInit(imgData, bgData, margin);
	textUI = new UIText();
	textUI->Init(nullptr, rectTransform);
}
