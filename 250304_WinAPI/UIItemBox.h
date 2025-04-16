#pragma once
#include "IButton.h"
#include "UIButton.h"

namespace UI
{
	class UIIcon;
	class UIText;

	class UIItemBox : public UIButton
	{
	public:
		~UIItemBox() override = default;

		void Release() override;
		void Render(HDC hdc) override;

		void SetText2(const string& str);
		void SetItem(ImageData itemImgData);

	protected:
		void ResourceInit(ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 }) override;

	protected:
		UIText* textUI2;
		UIIcon* itemIcon;
	};

}
