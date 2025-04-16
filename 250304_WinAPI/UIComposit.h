#pragma once
#include "UIObject.h"

namespace UI
{

	class UIComposit : public UIObject
	{
	public:
		virtual ~UIComposit() override = default;
		void Init(RECT rect) override;
		void Init(int dx, int dy, int width, int height) override;

	protected:
		virtual void ResourceInit() = 0;
	};

}