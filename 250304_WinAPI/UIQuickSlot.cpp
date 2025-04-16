#include "UIQuickSlot.h"
#include "UIButton.h"
#include "UIItemBox.h"
#include "CommonFunction.h"


using namespace UI;

void UIQuickSlot::Release()
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

void UIQuickSlot::Update()
{
}

void UIQuickSlot::Render(HDC hdc)
{
	if (BackpackBtn)
	{
		BackpackBtn->Render(hdc);
	}
	if (SlepBtn)
	{
		SlepBtn->Render(hdc);
	}
	if (SearchBtn)
	{
		SearchBtn->Render(hdc);
	}
	for (int i = 0; i < ItemBox.size(); ++i)
	{
		ItemBox[i]->Render(hdc);
	}
}

void UIQuickSlot::ResourceInit()
{
	for (int i = 0; i < 3; ++i)
	{
		auto itemBox = new UIItemBox();
		itemBox->Init(CaculateRelativeRECT(rectTransform, { 55 * i, 8, 55 * (i + 1), 67 }),
			ImageData{ "item_quick", L"assets/interfaces/item_quick.bmp", true, RGB(255, 255, 255) });

		ItemBox.push_back(itemBox);
	}
	SearchBtn = new UIButton();
	SearchBtn->Init(CaculateRelativeRECT(rectTransform, { 165, 0, 218, 67 }),
		ImageData{ "search_quick", L"assets/interfaces/search_quick.bmp", true, RGB(255, 255, 255) });
	SlepBtn = new UIButton();
	SlepBtn->Init(CaculateRelativeRECT(rectTransform, { 218, 0, 271, 67 }),
		ImageData{ "sleep_quick", L"assets/interfaces/sleep_quick.bmp", true, RGB(255, 255, 255) });
	BackpackBtn = new UIButton();
	BackpackBtn->Init(CaculateRelativeRECT(rectTransform, {271, 0, 337, 67}), 
		ImageData{ "inven_quick", L"assets/interfaces/inven_quick.bmp", true, RGB(255, 255, 255) });
}
