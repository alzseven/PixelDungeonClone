#include "UIQuickSlot.h"
#include "UIButton.h"


using namespace UI;

void UI::UIQuickSlot::Init(RECT rect)
{
	UIObject::Init(rect);

	ResourceInit();
}

void UI::UIQuickSlot::Init(int dx, int dy, int width, int height)
{
	UIObject::Init(dx, dy, width, height);

	ResourceInit();
}

void UI::UIQuickSlot::Release()
{
	if (BackpackBtn)
	{
		BackpackBtn->Release();
		delete BackpackBtn;
		BackpackBtn = nullptr;
	}
	if (SlepBtn)
	{
		SlepBtn->Release();
		delete SlepBtn;
		SlepBtn = nullptr;
	}
	if (SearchBtn)
	{
		SearchBtn->Release();
		delete SearchBtn;
		SearchBtn = nullptr;
	}
}

void UI::UIQuickSlot::Update()
{
}

void UI::UIQuickSlot::Render(HDC hdc)
{
	BackpackBtn->Render(hdc);
	//SlepBtn->Render(hdc);
	//SearchBtn->Render(hdc);
	//RenderRectAtCenter(hdc, centerX, centerY, width, height);
	//RenderStar(hdc, centerX, centerY);
	//RenderEllipseAtCenter(hdc, centerX, centerY, width, height);
}

void UI::UIQuickSlot::ResourceInit()
{
	BackpackBtn = new UIButton();
	BackpackBtn->Init({ 0, 0, 100, 100 }, {});
	BackpackBtn->SetPos(centerX - (width / 2), centerY - (height / 2));
	BackpackBtn->SetScale(100, 100);
}
