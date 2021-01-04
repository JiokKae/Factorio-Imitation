#include "Inventory.h"

void Inventory::AddItem(string strkey, int amount)
{
	map<string, int>::iterator it = mapInventory.find(strkey);
	if (it != mapInventory.end())
	{
		it->second += amount;
	}
	else
	{
		mapInventory.insert(make_pair(strkey, amount));
	}
}

vector<ItemInfo> Inventory::GetItemInfoArray()
{
	map<string, int>::iterator it;
	vector<ItemInfo> vecItemInfo;
	for (it = mapInventory.begin(); it != mapInventory.end(); it++)
	{
		vecItemInfo.push_back({ it->first, it->second });
	}
	return vecItemInfo;
}
