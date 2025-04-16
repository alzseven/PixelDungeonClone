#pragma once
#include "UIComposit.h"

namespace UI
{
	class UIIcon;
	class UIText;
	class UIButton;

	class UIMenueBtns : public UIComposit
	{
	public:
		~UIMenueBtns() override = default;

		void Init(RECT rect) override;
		void Init(int dx, int dy, int width, int height) override;
		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;

	protected:
		void ResourceInit();

	public:
		/* Child UI */
		UIIcon* stairIcon;
		UIText* stairTextUI;
		UIButton* guideBookBtn;
		UIButton* menuBtn;
	};

}