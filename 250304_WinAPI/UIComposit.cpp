#include "UIComposit.h"

using namespace UI;

void UI::UIComposit::Init(UIObject* parent, RECT rect)
{
	UIObject::Init(parent, rect);

	ResourceInit();
}

void UI::UIComposit::Init(UIObject* parent, int dx, int dy, int width, int height)
{
	UIObject::Init(parent, dx, dy, width, height);

	ResourceInit();
}
