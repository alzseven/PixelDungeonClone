#pragma once
#include "config.h"

class Entity;
class D2DImage;

enum class ItemType
{
	CONSUMABLE,
	EQUIPMENT
};

class Item
{
protected:
	string name;
	FPOINT position;
	ItemType type;

	D2DImage* image;
	int imageIdX, imageIdY; // items.png에서의 해당 아이템 인덱스 좌표
public:
	Item();
	virtual ~Item();

	virtual void Use(Entity* user);
	virtual void Render(HDC hdc) = 0;

	inline string GetName() { if (name.empty()) return ""; return name; }
	D2DImage* GetImage() { if (image) return image; }
	FPOINT GetPosition() { return position; }
	int GetImgIdX() { return imageIdX; }
	int GetImgIdY() { return imageIdY; }
	ItemType GetType() { return type; }
};

