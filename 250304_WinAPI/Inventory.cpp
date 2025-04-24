#include "Inventory.h"
#include "Item.h"

#include "UIManager.h"

Inventory::Inventory(Entity* owner) : owner(owner)
{
}

Inventory::~Inventory()
{
}

void Inventory::AddItem(Item* item)
{
	string key = item->GetName();

    // key 값에 해당하는 인덱스가 있으면 인벤토리에서
    // 해당 인덱스 count 증가
	auto it = indexMap.find(key);

    // 장비... 같은 이름이면 indexMap 사용 불가,,, 같은 장비 못먹는다?
    if (it != indexMap.end() && indexMap[key] != -1)
    {
        if (item->GetType() == ItemType::CONSUMABLE)
        {
            items[it->second].count++;
        }
        return;
    }

    // 없으면 빈 슬롯 찾아 할당
    for (int i = 3; i < (int)items.size(); ++i) 
    {
        if (items[i].count == 0) 
        {
            items[i].item = item;
            items[i].count = 1;
            indexMap[key] = i;
            return;
        }
    }
}

slot Inventory::GetSlot(int idx)
{
    return items[idx];
}
Item* Inventory::GetItem(string name)
{
    if (indexMap[name] == -1) return nullptr;
    int index = indexMap[name];
	if (index < 0 || index >= items.size()) return nullptr;

	return items[index].item;
}

void Inventory::UseItem(int idx)
{
    UIManager::GetInstance()->SendLog((L"Item 사용 " + to_wstring(idx)), D2D1::ColorF(D2D1::ColorF::Blue));

    
    if (items[idx].item == nullptr) return;
    UseItem(items[idx].item->GetName());

    //auto str = items[idx].item->GetName();
    //int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    //std::wstring wstr(size_needed, 0);
    //MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
    //UIManager::GetInstance()->SendLog((L"Item 사용 " + wstr), D2D1::ColorF(D2D1::ColorF::Blue));
}
void Inventory::UseItem(string name)
{
    if (indexMap[name] == -1) return;
    int index = indexMap[name];
	if (index < 0 || index >= items.size()) return;
    if (items[index].count <= 0) return;

    Item* item = items[index].item;
    item->Use(owner);
    items[index].count--;
     
    if (items[index].count <= 0)
    {
        // 인벤토리에서 제거
        items[index].count = 0;
        delete items[index].item;
        items[index].item = nullptr;

        // 인덱스 정보 -1로 초기화
        indexMap[name] = -1;
    }

	return;
}
