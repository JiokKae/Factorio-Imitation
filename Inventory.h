#pragma once
#include "GameNode.h"
#include "ItemEnum.h"

class Inventory : public GameNode
{
	map<ItemEnum, ItemInfo*> mapInventory;

public:
	void AddItem(ItemInfo* itemInfo);
	vector<ItemInfo*> GetItemInfoArray();
};

