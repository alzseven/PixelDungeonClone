#pragma once
#include "Item.h"
class Consumable : public Item
{
public:
	Consumable() { this->type = ItemType::CONSUMABLE; }
	virtual ~Consumable() {};
};

