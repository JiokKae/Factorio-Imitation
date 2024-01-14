#pragma once
#include "framework.h"
#include "ItemId.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	void AddItem(const ItemInfo& itemInfo);
	std::vector<ItemInfo*> GetItemInfoArray() const;
	bool GetItem(int itemId, ItemInfo* destItemInfo);
	ItemInfo* FindItem(int itemId);
private:
	std::map<ItemId, ItemInfo*> inventory;
};
