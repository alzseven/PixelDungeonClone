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

	void UITextSlider::SetMaxValue(float Value)
	{
		UISlider::SetMaxValue(Value);
		if (!valueText) return;
		string ValueString = (to_string((int)goalValue) + " / " + to_string((int)maxValue));
		valueText->SetText(ValueString);
	}

	void UITextSlider::SetValue(float Value)
	{
		UISlider::SetValue(Value);
		if (!valueText) return;
		string ValueString = (to_string((int)goalValue) + " / " + to_string((int)maxValue));
		valueText->SetText(ValueString);
	}

	void UITextSlider::ResourceInit(ImageData imgData, ImageData bgData, ImageData handleData, RECT margin)
	{
		UISlider::ResourceInit(imgData, bgData, handleData, margin);

		valueText = new UIText();
		string ValueString = (to_string((int)goalValue) + " / " + to_string((int)maxValue));
		valueText->Init(nullptr, { rectTransform.left + margin.left, rectTransform.top + margin.top + 10, 
			rectTransform.right - margin.right, rectTransform.bottom - margin.bottom  }
		, ValueString, RGB(255, 255, 255));
	}
}