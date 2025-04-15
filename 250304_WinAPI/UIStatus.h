#pragma once
#include "UIComposit.h"

class Image;

namespace UI
{
	class UIIcon;
	class UIText;
	class UISlider;

	class UIStatus : public UIComposit
	{
	public:
		~UIStatus() override = default;

		void Init(RECT rect) override;
		void Init(int dx, int dy, int width, int height) override;
		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;

	protected:
		void ResourceInit();

	public:
		UIIcon* icon;
		UIText* levelText;
		UISlider* hpBar;
		UISlider* expBar;
	};

}