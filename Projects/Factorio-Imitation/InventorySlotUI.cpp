#include "InventorySlotUI.h"
#include "../GLFramework/Image/GLImage.h"
#include "../GLFramework/TextRenderer/TextRenderer.h"
#include "HandUI.h"
#include "Character.h"
#include "Inventory.h"
HRESULT InventorySlotUI::Init(int x, int y)
{
	SlotUI::Init();

	handImage = new GLImage("Icons/Hand", 1, 1, 32, 32);

	localPosition.x = x * (GetWidth() + 2);
	localPosition.y = -y * (GetHeight() + 2);

	return S_OK;
}

void InventorySlotUI::Release()
{
	SlotUI::Release();

	SAFE_DELETE(handImage);
}

void InventorySlotUI::OnClick(int key)
{
	ItemInfo* hand = UIManager::GetSingleton()->GetLpHandUI()->GetHandItem();
	switch (key)
	{
	//슬롯을 좌클릭 했을 때
	case VK_LBUTTON:	
		// 핸드가 없다면
		if (hand->IsEmpty())	
		{
			// 슬롯에 아이템이 있다면 
			if (this->itemInfo)
			{
				// 아이템 전부 핸드로
				itemInfo->MoveAllItemTo(hand);
			}
		}
		// 핸드가 있다면
		else
		{
			// 핸드를 전부 주고
			EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(ItemInfo(hand->id, hand->amount));
			hand->amount = 0;

			// 슬롯에 아이템이 있다면 
			if (this->itemInfo)
			{
				// 아이템 전부 핸드로
				itemInfo->MoveAllItemTo(hand);
			}
		}
		break;

	// 슬롯을 우클릭 했을 때
	case VK_RBUTTON:
		// 핸드가 있다면
		if (hand->amount)
		{
			// 핸드에서 하나만 슬롯으로
			EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(ItemInfo(hand->id, 1));
			hand->amount -= 1;
		}
		// 핸드가 없다면
		else
		{
			// 슬롯에 아이템이 있다면 
			if (itemInfo)
			{
				// 슬롯의 반만 핸드로
				hand->id = itemInfo->id;
				hand->amount = itemInfo->amount / 2;
				itemInfo->amount -= itemInfo->amount / 2;
			}
		}
		break;
	}
}
// 이 주석을 지워주세요