#pragma once
#include "config.h"

class Item;
class Entity;
struct slot
{
	int count;
	Item* item;
};

class Inventory
{
private:
	vector<slot> items{ 25, {0, nullptr} };		// 실제 인벤토리
	unordered_map<string, int> indexMap;		// first : 아이템 name, second : 해당 아이템 인덱스번호

	Entity* owner;
public:
	Inventory(Entity* owner);
	~Inventory();

	void AddItem(Item* item);
	void UseItem(int idx);
	void UseItem(string name);

	slot GetSlot(int idx);

	// 필요 없을 것 같으면 삭제 ㄱㄱ
	Item* GetItem(string name);
};

