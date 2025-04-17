#pragma once
#include "UIObject.h"

namespace UI
{

	class UIText : public UIObject
	{
	public:
		~UIText() override = default;
		void Init(UIObject* parent, RECT rect, const string& text = "", COLORREF textColor = RGB(0, 0, 0));
		void Init(UIObject* parent, int dx, int dy, int width, int height, const string& text = "", COLORREF textColor = RGB(0, 0, 0));
		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;
		void SetText(const string& text) { this->text = text; }
		void SetTextColor(COLORREF color) { this->textColor = color; }

	protected:
		string text;
		COLORREF textColor{ RGB(0, 0, 0) };
		Image* bg;
	};

}