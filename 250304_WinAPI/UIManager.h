#pragma once
#include "UIObject.h"

namespace UI
{
	class UIStatus;
	class UIQuickSlot;
	class UIMenueBtns;
	class UIDefeat;

	inline void RenderIsVisible(HDC hdc, UIObject* object)
	{
		if (object->isVisible)
		{
			object->Render(hdc);
		}
	}

	class UIManager
	{
	public:
		void Init();
		void Release();
		void Update();
		void Render(HDC hdc);

		void Reset();

		void SetStatus(StatInfo statInfo);
		void SetItemInfo(/* Todo */);

		void ShowDefeat(bool flag);


	protected:
		void ResourceInit();

	protected:
		UIStatus* statToolbar;
		UIQuickSlot* quickSlot;
		UIMenueBtns* menueToolbar;
		UIDefeat* defeatUI;
	};

}