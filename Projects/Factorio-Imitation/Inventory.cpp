#include "Inventory.h"

HRESULT Inventory::Init()
{
	return S_OK;
}

void Inventory::Release()
{
	auto it = mapInventory.begin();
	while (it != mapInventory.end())
	{
		SAFE_DELETE(it->second);
		it = mapInventory.erase(it);
	}
	mapInventory.clear();
}

void Inventory::AddItem(ItemInfo* disposableItemInfo)
{
	auto it = mapInventory.find((ItemEnum)disposableItemInfo->id);
	if (it != mapInventory.end())
	{
		it->second->amount += disposableItemInfo->amount;
		delete disposableItemInfo;
	}
	else
	{
		mapInventory.insert(make_pair((ItemEnum)disposableItemInfo->id, disposableItemInfo));
	}
}

vector<ItemInfo*> Inventory::GetItemInfoArray()
{
	vector<ItemInfo*> vecItemInfo;
	for (auto it = mapInventory.begin(); it != mapInventory.end(); it++)
	{
		if(!it->second->IsEmpty())
			vecItemInfo.push_back(it->second);
	}
	return vecItemInfo;
}

bool Inventory::GetItem(int itemId, ItemInfo* destItemInfo)
{
	if (destItemInfo->IsEmpty())
	{
		auto it = mapInventory.find((ItemEnum)itemId);
		if (it != mapInventory.end())
		{
			it->second->MoveAllItemTo(destItemInfo);
		}
	}

	return false;
}

ItemInfo* Inventory::FindItem(int itemId)
{
	auto it = mapInventory.find((ItemEnum)itemId);
	if (it != mapInventory.end())
	{
		return it->second;
	}
	else
		return nullptr;
}
