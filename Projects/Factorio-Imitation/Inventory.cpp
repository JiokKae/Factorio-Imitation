#include "Inventory.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
	for (auto& [id, item] : inventory)
	{
		SAFE_DELETE(item);
	}
}

void Inventory::AddItem(const ItemInfo& item)
{
	auto inventoryItem = FindItem(item.id);
	if (inventoryItem)
	{	
		inventoryItem->amount += item.amount;
		return;
	}

	inventory.emplace(static_cast<ItemEnum>(item.id), new ItemInfo(item.id, item.amount));
}

vector<ItemInfo*> Inventory::GetItemInfoArray()
{
	vector<ItemInfo*> vecItemInfo;
	for (auto& [id, item] : inventory)
	{
		if (item->IsEmpty() == false)
			vecItemInfo.push_back(item);
	}
	return vecItemInfo;
}

bool Inventory::GetItem(int itemId, ItemInfo* destItem)
{
	if (destItem->IsEmpty() == false)
	{
		return false;
	}

	auto item = FindItem(itemId);
	if (item == nullptr)
	{
		return false;
	}

	item->MoveAllItemTo(destItem);

	return true;
}

ItemInfo* Inventory::FindItem(int itemId)
{
	auto it = inventory.find(static_cast<ItemEnum>(itemId));
	if (it == inventory.end())
	{
		return nullptr;
	}

	return it->second;
}
