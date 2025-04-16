#pragma once
#include "UIIcon.h"
#include "IButton.h"

namespace UI
{
	class UIText;

	class UIButton : public UIIcon
	{
	public :
		virtual ~UIButton() = default;

		void Init(RECT rect, ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 });
		void Init(int dx, int dy, int width, int height,
			ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 });
		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;

		void SetText(string& txt);

	protected:
		void ResourceInit(ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 }) override;

	protected:
		UIText* textUI;
		COLORREF bgColorRGB{ RGB(0, 0, 0) };
	};

}
