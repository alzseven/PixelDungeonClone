#pragma once
#include "IButton.h"
#include "UIIcon.h"


namespace UI
{
	class UIText;

	class UIItemBox : public UIIcon, public IButton
	{
	public:
		~UIItemBox() override = default;

	protected:
		UIText* count;
		UIText* upgradeValue;
	};

}
