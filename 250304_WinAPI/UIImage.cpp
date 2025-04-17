#include "UIImage.h"
#include "Image.h"

using namespace UI;

void UI::UIImage::Init(UIObject* parent, RECT rect, ImageData imgData, RECT margin)
{
	UIObject::Init(parent, rect);

	ResourceInit(imgData, margin);
}

void UI::UIImage::Init(UIObject* parent, int dx, int dy, int width, int height, ImageData imgData, RECT margin)
{
	UIObject::Init(parent, dx, dy, width, height);

	ResourceInit(imgData, margin);
}

void UI::UIImage::Release()
{
	if (img)
	{
		img = nullptr;
	}
}

void UI::UIImage::Update()
{
}

void UI::UIImage::Render(HDC hdc)
{
	img->Render(hdc, rectTransform.left + margin.left, rectTransform.top + margin.top);
}

void UI::UIImage::ResourceInit(ImageData imgData, RECT margin)
{
	img = ImageManager::GetInstance()->AddImage(imgData.keyName, imgData.filePath,
		width, height, imgData.isTransparent, imgData.transColor);
	SetMargin(margin);
}
