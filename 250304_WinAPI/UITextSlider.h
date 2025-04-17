#pragma once
#include "UISlider.h"

namespace UI
{
	class UIText;

	class UITextSlider : public UISlider
	{
	public:
		~UITextSlider() override = default;
        void Release() override;
        void Render(HDC hdc) override;

		void SetMaxValue(float value) override;
		void SetValue(float value) override;

    protected:
        void ResourceInit(ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, ImageData handleData = { "", L"", 0, 0 },
            RECT margin = { 0,0,0,0 });

	protected:
		UIText* valueText;
	};

}