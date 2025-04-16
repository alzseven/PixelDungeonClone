#pragma once
#include "UIComposit.h"

namespace UI
{
	class UISlider;
	class UIMopHPBar : public UIComposit
	{
	public:
		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;
	protected:
		void ResourceInit() override;

	protected:
		UISlider* hpBar;
	};

}