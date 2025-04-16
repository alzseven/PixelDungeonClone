#pragma once
#include "UIComposit.h"

class Image;

namespace UI
{
	class UIIcon;
	class UITextBox;
	class UITextSlider;

	class UIStatus : public UIComposit
	{
	public:
		~UIStatus() override = default;

		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;

		void SetText(const string& text);

	protected:
		void ResourceInit() override;

	public:
		/* Child UI */
		UIIcon* icon;
		UITextBox* levelTextUI;
		UITextSlider* hpBar;
		UITextSlider* expBar;

	};

}