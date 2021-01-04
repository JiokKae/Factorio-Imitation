#pragma once
#include "GameNode.h"

struct ItemInfo {
	string name;
	int amount;
};

class Inventory : public GameNode
{
	map<string, int> mapInventory;

public:
	void AddItem(string strkey, int amount);
	vector<ItemInfo> GetItemInfoArray();
};

