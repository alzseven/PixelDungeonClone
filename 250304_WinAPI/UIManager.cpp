#include "UIManager.h"
#include "UIStatus.h"
#include "UIMenueBtns.h"
#include "UIQuickSlot.h"
#include "UIDefeat.h"

using namespace UI;

void UI::UIManager::Init()
{
	ResourceInit();
}

void UI::UIManager::Release()
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

void UI::UIManager::Update()
{
	statToolbar->Update();
	menueToolbar->Update();
	quickSlot->Update();
	defeatUI->Update();
}

void UI::UIManager::Render(HDC hdc)
{
	RenderIsVisible(hdc, statToolbar);
	RenderIsVisible(hdc, quickSlot);
	RenderIsVisible(hdc, menueToolbar);
	RenderIsVisible(hdc, defeatUI);
}

void UI::UIManager::Reset()
{
	Release();
	Init();
}

void UI::UIManager::SetStatus(StatInfo statInfo)
{
	if (statToolbar == nullptr) return;

	statToolbar->SetStatus(statInfo);
}

void UI::UIManager::ShowDefeat(bool flag)
{
	defeatUI->isVisible = flag;
}

void UI::UIManager::SetItemInfo()
{
	if (quickSlot == nullptr) return;
}

void UI::UIManager::ResourceInit()
{
	statToolbar = new UIStatus;
	quickSlot = new UIQuickSlot;
	menueToolbar = new UIMenueBtns;
	defeatUI = new UIDefeat;

	statToolbar->Init(nullptr, { 0, 623, 335, 720 });
	quickSlot->Init(nullptr, { 743,653, 337, 720 });
	menueToolbar->Init(nullptr, { 920, 0, 1080, 56 });
	defeatUI->Init(nullptr, {0, 0, WINSIZE_X, WINSIZE_Y});
	defeatUI->isVisible = false;
}