#pragma once
#include "UIObject.h"

namespace UI
{

	class UIComposit : public UIObject
	{
	public:
		virtual ~UIComposit() override = default;
	};

}