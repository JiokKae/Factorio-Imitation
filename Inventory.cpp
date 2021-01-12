#include "Inventory.h"

void Inventory::AddItem(ItemInfo* itemInfo)
{
	map<ItemEnum, ItemInfo*>::iterator it = mapInventory.find((ItemEnum)itemInfo->id);
	if (it != mapInventory.end())
	{
		it->second->amount += itemInfo->amount;
		delete itemInfo;
	}
	else
	{
		mapInventory.insert(make_pair((ItemEnum)itemInfo->id, itemInfo));
	}
}

vector<ItemInfo*> Inventory::GetItemInfoArray()
{
	map<ItemEnum, ItemInfo*>::iterator it;
	vector<ItemInfo*> vecItemInfo;
	for (it = mapInventory.begin(); it != mapInventory.end(); it++)
	{
		if(it->second->amount != 0)
			vecItemInfo.push_back(it->second);
	}
	return vecItemInfo;
}
