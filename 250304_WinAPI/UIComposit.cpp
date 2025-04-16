#include "UIComposit.h"

using namespace UI;

void UI::UIComposit::Init(RECT rect)
{
	UIObject::Init(rect);

	ResourceInit();
}

void UI::UIComposit::Init(int dx, int dy, int width, int height)
{
	UIObject::Init(dx, dy, width, height);

	ResourceInit();
}
