#pragma once
#include "UIObject.h"

namespace UI
{
	class UIText;

	class UITextBox : public UIObject
	{
	public:
		~UITextBox() override = default;

		void Init(RECT rect, const string& text = "", ImageData bgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 });
		void Init(int dx, int dy, int width, int height, const string& text="", ImageData bgData = {"", L"", 0, 0}, RECT margin = { 0,0,0,0 });
		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;

		void SetText(const string& text);

	protected:
		void ResourceInit(const string& text="", ImageData bgData = {"", L"", 0, 0}, RECT margin = { 0,0,0,0 });

	protected:
		UIText* textUI;
		Image* bg;

	};

}