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

		void SetPos(int dx, int dy) override;
		void SetMaxHP(int hp);
		void SetHP(int hp);
	protected:
		void ResourceInit() override;

	protected:
		UISlider* hpBar;
	};

}