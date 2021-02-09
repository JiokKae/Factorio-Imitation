#include "Container.h"

HRESULT Container::Init(int x, int y, DIRECTION direction, bool temp)
{
	vecInventory.resize(inventorySize);
	return Structure::Init(x, y, direction, temp);
}

void Container::Release()
{
	vecInventory.clear();
	Structure::Release();
}

bool Container::InputItem(ItemInfo* inputtedItem, glm::vec2 pos)
{
	for (int i = 0; i < inventorySize; i++)
	{
		if (vecInventory[i].id == inputtedItem->id)
		{
			ItemInfo* item = &vecInventory[i];
			int stackSize = g_itemSpecs[item->id].stackSize;
			// 아이템이 들어갈 수 있다면
			if (item->amount < stackSize)
			{
				// 한번에 다 못 넣는다면
				if (item->amount + inputtedItem->amount > stackSize)
				{
					inputtedItem->amount -= stackSize - item->amount;
					item->amount = stackSize;
				}
				// 한번에 다 넣는다면
				else
				{
					item->amount += inputtedItem->amount;
					inputtedItem->amount = 0;
					return true;
				}
			}
		}
		
	}
	for (int i = 0; i < inventorySize; i++)
	{
		if (vecInventory[i].amount == 0)
		{
			inputtedItem->MoveAllItemTo(&vecInventory[i]);
			sort(vecInventory.begin(), vecInventory.end(), greater<ItemInfo>());
			return true;
		}
	}
	return false;
}

string Container::ToString()
{
	string strBuf;
	char szBuf[128];
	for (int i = 0; i < inventorySize; i++)
	{
		ItemInfo* item = &vecInventory[i];
		if (item->amount != 0)
		{
			wsprintf(szBuf, "\n %d\t10\t x %s", item->amount, g_itemSpecs[item->id].name.c_str());
			strBuf += string(szBuf);
		}
	}

	return Structure::ToString() + strBuf;
}
