#include "UITextSlider.h"
#include "UIText.h"
#include "CommonFunction.h"

namespace UI
{

	void UITextSlider::Release()
	{
		UISlider::Release();

		if (valueText)
		{
			valueText->Release();
			delete valueText;
			valueText = nullptr;
		}
	}

	void UITextSlider::Render(HDC hdc)
	{
		UISlider::Render(hdc);

		if (valueText)
		{
			valueText->Render(hdc);
		}
	}

	void UITextSlider::SetMaxHP(float hp)
	{
		UISlider::SetMaxHP(hp);
		if (!valueText) return;
		string hpString = (to_string((int)goalHP) + " / " + to_string((int)maxHP));
		valueText->SetText(hpString);
	}

	void UITextSlider::SetHP(float hp)
	{
		UISlider::SetHP(hp);
		if (!valueText) return;
		string hpString = (to_string((int)goalHP) + " / " + to_string((int)maxHP));
		valueText->SetText(hpString);
	}

	void UITextSlider::ResourceInit(ImageData imgData, ImageData bgData, ImageData handleData, RECT margin)
	{
		UISlider::ResourceInit(imgData, bgData, handleData, margin);

		valueText = new UIText();
		string hpString = (to_string((int)goalHP) + " / " + to_string((int)maxHP));
		valueText->Init({ rectTransform.left + margin.left, rectTransform.top + margin.top + 10, 
			rectTransform.right - margin.right, rectTransform.bottom - margin.bottom  }
		, hpString, RGB(255, 255, 255));
	}
}