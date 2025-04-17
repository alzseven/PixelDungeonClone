#include "UIIcon.h"
#include "Image.h"
#include "ImageManager.h"

using namespace UI;

void UIIcon::Init(UIObject* parent, RECT rect, ImageData imgData, ImageData bgData, RECT margin)
{
	UIObject::Init(parent, rect);

	ResourceInit(imgData, bgData, margin);
}

void UIIcon::Init(UIObject* parent, int dx, int dy, int width, int height, ImageData imgData, ImageData bgData, RECT margin)
{
	UIObject::Init(parent, dx, dy, width, height);

	ResourceInit(imgData, bgData, margin);
}

void UIIcon::Release()
{
	if (icon)
	{
		icon = nullptr;
	}

	if (bg)
	{
		bg = nullptr;
	}
}

void UIIcon::Update()
{
}

void UIIcon::Render(HDC hdc)
{
	if (bg)
	{
		bg->Render(hdc, rectTransform.left, rectTransform.top);
	}
	if (icon)
	{
		icon->Render(hdc, rectTransform.left + margin.left, rectTransform.top + margin.top);
	}
}

void UIIcon::ResourceInit( ImageData imgData, ImageData bgData, RECT margin )
{
	this->margin = margin;

	if (imgData.keyName != "")
	{
		icon = ImageManager::GetInstance()->AddImage(imgData.keyName, imgData.filePath,
			width - margin.left * 2, height - margin.top * 2, imgData.isTransparent, imgData.transColor);
	}

	if (bgData.keyName != "")
	{
		bg = ImageManager::GetInstance()->AddImage(bgData.keyName, bgData.filePath, width, height, bgData.isTransparent, bgData.transColor);
		return;
	}
}
