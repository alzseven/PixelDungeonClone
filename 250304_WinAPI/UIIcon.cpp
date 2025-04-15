#include "UIIcon.h"
#include "Image.h"
#include "ImageManager.h"

using namespace UI;

void UIIcon::Init( RECT rect, ImageData imgData, ImageData bgData, RECT margin)
{
	UIObject::Init(rect);

	ResourceInit(imgData, bgData, margin);
}

void UIIcon::Init( int dx, int dy, int width, int height, ImageData imgData, ImageData bgData, RECT margin)
{
	UIObject::Init(dx, dy, width, height);

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
	Rectangle(hdc, rectTransform.left, rectTransform.top, rectTransform.right, rectTransform.bottom);
	icon->Render(hdc, rectTransform.left + margin.left, rectTransform.top + margin.top);
	if (bg)
	{
		bg->Render(hdc, rectTransform.left, rectTransform.top);
	}
}

void UIIcon::ResourceInit( ImageData imgData, ImageData bgData, RECT margin )
{
	this->margin = margin;

	icon = ImageManager::GetInstance()->AddImage(imgData.keyName, imgData.filePath, 
		width-margin.left, height-margin.top, imgData.isTransparent, imgData.transColor);
	if (icon == nullptr)
	{
		wstring failedFilePath = imgData.filePath;
		OutputDebugString((L"Icon Load Failed : ( " + failedFilePath + L" )\n").c_str());
	}

	if (bgData.keyName != "")
	{
		bg = ImageManager::GetInstance()->AddImage(bgData.keyName, bgData.filePath, width, height, bgData.isTransparent, bgData.transColor);
		return;
	}
}
