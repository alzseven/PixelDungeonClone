#pragma once
#include "UIObject.h"

namespace UI
{
	class UIStatus;
	class UIQuickSlot;
	class UIMenueBtns;

	class UIView : public UIObject
	{
	public:
		~UIView() override = default;
		
		void Init(RECT rect) override;
		void Init(int dx = 0, int dy = 0, int width = 0, int height = 0) override;
		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;

	protected:
		void ResourceInit();

	protected:
		UIStatus* statToolbar;
		UIQuickSlot* quickSlot;
		UIMenueBtns* menueToolbar;
	};

}