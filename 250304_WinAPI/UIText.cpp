#include "UIText.h"

void UI::UIText::Init(RECT rect, const string& text, COLORREF textColor)
{
	UIObject::Init(rect);
	this->text = text;
	this->textColor = textColor;
}

void UI::UIText::Init(int dx, int dy, int width, int height, const string& text, COLORREF textColor)
{
	UIObject::Init(dx, dy, width, height);
	this->text = text;
	this->textColor = textColor;
}

void UI::UIText::Release()
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

void UI::UIText::Update()
{
}

void UI::UIText::Render(HDC hdc)
{
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
	TextOutA(hdc, rectTransform.left + 5, rectTransform.top + 5, text.c_str(), text.length());
}
