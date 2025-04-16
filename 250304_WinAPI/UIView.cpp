#include "UIView.h"
#include "UIStatus.h"
#include "UIMenueBtns.h"
#include "UIQuickSlot.h"
#include "UIDefeat.h"

using namespace UI;

void UI::UIView::Init(RECT rect)
{
	UIObject::Init(rect);

	ResourceInit();
}

void UI::UIView::Init(int dx, int dy, int width, int height)
{
	UIObject::Init(dx,dy,width,height);

	ResourceInit();
}

void UI::UIView::Release()
{
	if (statToolbar)
	{
		statToolbar->Release();
		delete statToolbar;
		statToolbar = nullptr;
	}
	if (menueToolbar)
	{
		menueToolbar->Release();
		delete menueToolbar;
		menueToolbar = nullptr;
	}
	if (quickSlot)
	{
		quickSlot->Release();
		delete quickSlot;
		quickSlot = nullptr;
	}
	if (defeatUI)
	{
		defeatUI->Release();
		delete defeatUI;
		defeatUI = nullptr;
	}
}

void UI::UIView::Update()
{
	statToolbar->Update();
	menueToolbar->Update();
	quickSlot->Update();
	defeatUI->Update();
}

void UI::UIView::Render(HDC hdc)
{
	statToolbar->Render(hdc);
	menueToolbar->Render(hdc);
	quickSlot->Render(hdc);
	defeatUI->Render(hdc);
}

void UI::UIView::ResourceInit()
{
	statToolbar = new UIStatus;
	quickSlot = new UIQuickSlot;
	menueToolbar = new UIMenueBtns;
	defeatUI = new UIDefeat;

	statToolbar->Init({ 0, 623, 335, 720 });
	quickSlot->Init({ 743,653, 337, 720 });
	menueToolbar->Init({ 920, 0, 1080, 56 });
	defeatUI->Init(rectTransform);
}