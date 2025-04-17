#include "UIText.h"

using namespace UI;

void UIText::Init(UIObject* parent, RECT rect, const string& text, COLORREF textColor)
{
	UIObject::Init(parent, rect);
	this->text = text;
	this->textColor = textColor;
}

void UIText::Init(UIObject* parent, int dx, int dy, int width, int height, const string& text, COLORREF textColor)
{
	UIObject::Init(parent, dx, dy, width, height);
	this->text = text;
	this->textColor = textColor;
}

void UIText::Release()
{
	if (text != "")
	{
		text.clear();
	}
	if (textColor != RGB(0, 0, 0))
	{
		textColor = RGB(0, 0, 0);
	}
}

void UIText::Update()
{
}

void UIText::Render(HDC hdc)
{
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
	TextOutA(hdc, centerX, centerY, text.c_str(), text.length());
}
