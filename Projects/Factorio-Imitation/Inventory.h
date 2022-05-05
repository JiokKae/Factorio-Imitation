#pragma once
#include "framework.h"
#include "2DFramework/GameNode.h"
#include "ItemEnum.h"

class Inventory : public GameNode
{
	map<ItemEnum, ItemInfo*> mapInventory;

public:
	virtual HRESULT Init();
	virtual void Release();

	void AddItem(ItemInfo* itemInfo);
	vector<ItemInfo*> GetItemInfoArray();
	bool GetItem(int itemId, ItemInfo* destItemInfo);
	ItemInfo* FindItem(int itemId);
};

