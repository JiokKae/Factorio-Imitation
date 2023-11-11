#pragma once
#include "framework.h"
#include "ItemId.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	void AddItem(const ItemInfo& itemInfo);
	vector<ItemInfo*> GetItemInfoArray();
	bool GetItem(int itemId, ItemInfo* destItemInfo);
	ItemInfo* FindItem(int itemId);
private:
	std::map<ItemId, ItemInfo*> inventory;
};
