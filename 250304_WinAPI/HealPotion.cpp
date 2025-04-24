#include "HealPotion.h"
#include "D2DImageManager.h"
#include "D2DImage.h"
#include "Entity.h"

HealPotion::HealPotion(FPOINT position)
{
	this->position = position;
	this->name = "체력포션";
	this->imageIdX = 0;
	this->imageIdY = 21;
}

HealPotion::~HealPotion()
{
}

void HealPotion::Use(Entity* user)
{
	if (!user || !user->IsAlive())	return;
	
	user->Heal(healAmount);
}

void HealPotion::Render(HDC hdc)
{
	image->Middle_RenderFrameScale(position.x, position.y, 2, 2, 0, 22);
}
