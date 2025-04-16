#include "UIObject.h"

using namespace UI;



void UIObject::Init(RECT rect)
{
	centerX = (rect.right + rect.left) / 2;
	centerY = (rect.bottom + rect.top) / 2;
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	rectTransform = rect;
}

void UIObject::Init(int dx, int dy, int width, int height)
{
	this->centerX = dx;
	this->centerY = dy;
	this->width = width;
	this->height = height;

	UpdateRectTransform();
}

void UIObject::SetPos(int dx, int dy)
{
	this->centerX = dx;
	this->centerY = dy;

	UpdateRectTransform();
}

void UIObject::SetScale(int width, int height)
{
	this->width = width;
	this->height = height;

	UpdateRectTransform();
}

void UIObject::SetRect(RECT rect)
{
	this->rectTransform = rect;
	this->width = rect.right - rect.left;
	this->height = rect.bottom - rect.top;
	this->centerX = (rect.left + rect.right) / 2;
	this->centerY = (rect.top + rect.bottom) / 2;
}

void UIObject::SetRect(int dx, int dy, int width, int height)
{
	this->centerX = dx;
	this->centerY = dy;
	this->width = width;
	this->height = height;
	UpdateRectTransform();
}

void UIObject::UpdateRectTransform()
{
	int halfWidth = this->width / 2;
	int halfHeight = this->height / 2;
	this->rectTransform = { this->centerX - halfWidth, this->centerY - halfHeight,
		this->centerX + halfWidth, this->centerY + halfHeight };
}
