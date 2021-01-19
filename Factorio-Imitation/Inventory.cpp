#include "Inventory.h"

HRESULT Inventory::Init()
{
	return S_OK;
}

void Inventory::Release()
{
	map<ItemEnum, ItemInfo*>::iterator it = mapInventory.begin();
	while (it != mapInventory.end())
	{
		SAFE_DELETE(it->second);
		it = mapInventory.erase(it);
	}
	mapInventory.clear();
}

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
