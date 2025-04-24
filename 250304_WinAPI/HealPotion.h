#pragma once
#include "Consumable.h"


class HealPotion : public Consumable
{
private:
	int healAmount = 1;

public:
	HealPotion(FPOINT position);
	virtual ~HealPotion();

	void Use(Entity* user) override;
	void Render(HDC hdc) override;
};

