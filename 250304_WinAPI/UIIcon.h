#pragma once  
#include "UIObject.h"  

class Image;  

namespace UI  
{  
	class UIIcon : public UIObject  
	{  
	public:  
		UIIcon() = default;
		~UIIcon() override = default;  

		void Init(RECT rect, ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 });
		void Init(int dx, int dy, int width, int height  
			, ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 });
		void Release() override;  
		void Update() override;
		void Render(HDC hdc) override; 

	protected:
		void ResourceInit(ImageData imgData, ImageData bgData = { "", L"", 0, 0 }, RECT margin = { 0,0,0,0 });

	protected:  
		Image* icon;
		Image* bg;

		RECT margin;
	};  
}