#pragma once
#include "UIObject.h"

namespace UI
{
	class UIButton;
	class UIItemBox;

	class UIQuickSlot : public UIObject
	{
	public:
		~UIQuickSlot() override = default;

		void Init(RECT rect) override;
		void Init(int dx, int dy, int width, int height) override;
		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;

	protected:
		void ResourceInit();

	public:
		UIButton* BackpackBtn;
		UIButton* SlepBtn;
		UIButton* SearchBtn;
		vector<UIItemBox> ItemBox;
	};

}