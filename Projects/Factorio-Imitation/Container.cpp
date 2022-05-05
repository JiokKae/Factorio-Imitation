#include "Container.h"

HRESULT Container::Init(int x, int y, DIRECTION _direction, bool _temp)
{
	vecInventory.resize(inventorySize);
	return Structure::Init(x, y, _direction, _temp);
}

void Container::Release()
{
	vecInventory.clear();
	Structure::Release();
}

bool Container::InputItem(ItemInfo* inputtedItem, glm::vec2 /*pos*/)
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

bool Container::TakeOutItem(ItemInfo* outItem)
{
	for (int i = 0; i < inventorySize; i++)
	{
		if (vecInventory[i].amount != 0)
		{
			// 저장품의 개수가 반출 요구치를 넘으면
			if (vecInventory[i].amount >= outItem->amount)
			{
				vecInventory[i].amount -= outItem->amount;
				outItem->id = vecInventory[i].id;
			}
			// 저장품의 개수가 반출 요구치를 넘지 못하면
			else
				vecInventory[i].MoveAllItemTo(outItem);

			return true;
		}
	}
	return false;
}

string Container::ToString()
{
	string strBuf;
	//char szBuf[128];
	for (int i = 0; i < inventorySize; i++)
	{
		ItemInfo* item = &vecInventory[i];
		if (item->amount != 0)
		{
			strBuf += std::format("\n {}\t10\t x {}", item->amount, g_itemSpecs[item->id].name);
			//wsprintf(szBuf, "\n %d\t10\t x %s", item->amount, g_itemSpecs[item->id].name.c_str());
			//strBuf += string(szBuf);
		}
	}

	return Structure::ToString() + strBuf;
}
