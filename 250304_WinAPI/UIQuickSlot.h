#pragma once
#include "UIComposit.h"

namespace UI
{
	class UIButton;
	class UIItemBox;

	class UIQuickSlot : public UIComposit
	{
	public:
		~UIQuickSlot() override = default;

		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;

	protected:
		void ResourceInit() override;

	public:
		UIButton* BackpackBtn;
		UIButton* SlepBtn;
		UIButton* SearchBtn;
		vector<UIItemBox*> ItemBox;
	};

}