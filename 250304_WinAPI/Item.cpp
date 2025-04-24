#include "Item.h"
#include "D2DImageManager.h"

Item::Item()
{
	image = D2DImageManager::GetInstance()->AddImage("æ∆¿Ã≈€", L"Image\\Items.png", 16, 32);
}

Item::~Item()
{
}

void Item::Use(Entity* user)
{
}
