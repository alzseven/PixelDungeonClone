#pragma once
#include "UIObject.h"

class Image;

namespace UI
{

	class UIImage : public UIObject
	{
	public:
		~UIImage() override = default;

		void Init(UIObject* parent, RECT rect,
			ImageData imgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 });
		void Init(UIObject* parent, int dx, int dy, int width, int height
			, ImageData imgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 });
		void Release() override;
		void Update() override;
		void Render(HDC hdc) override;

		void SetMargin(RECT margin)
		{
			this->margin = margin;
		}

	protected:
		virtual void ResourceInit(ImageData imgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 });

	protected:
		Image* img;
		RECT margin;
	};

}