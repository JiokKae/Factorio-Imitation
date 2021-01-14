#include "InventorySlotUI.h"
#include "GLImage.h"
#include "TextRenderer.h"
#include "HandUI.h"
#include "Character.h"
#include "Inventory.h"
HRESULT InventorySlotUI::Init(int x, int y)
{
	SlotUI::Init();

	hand = new GLImage();
	hand->Init("Icons/Hand", 1, 1, 32, 32);

	localPosition.x = x * (GetWidth() + 2);
	localPosition.y = -y * (GetHeight() + 2);

	return S_OK;
}

void InventorySlotUI::Release()
{
	SlotUI::Release();

	SAFE_RELEASE(hand);
}

void InventorySlotUI::OnClick(int key)
{
	ItemInfo* hand = UIManager::GetSingleton()->GetLpHandUI()->GetHandItem();
	switch (key)
	{
	//������ ��Ŭ�� ���� ��
	case VK_LBUTTON:	
		//�ڵ尡 �ִٸ�
		if (hand->amount)	
		{
			// �ڵ带 ���� �ְ�
			EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(new ItemInfo(hand->id, hand->amount));
			hand->amount = 0;	
			
			// ���Կ� �������� �ִٸ� 
			if (itemInfo)
			{
				// ������ ���� �ڵ��
				hand->id = itemInfo->id;
				hand->amount = itemInfo->amount;
				itemInfo->amount = 0;
			}
		}
		// �ڵ尡 ���ٸ�
		else
		{
			// ���Կ� �������� �ִٸ� 
			if (itemInfo)
			{
				// ������ ���� �ڵ��
				hand->id = itemInfo->id;
				hand->amount = itemInfo->amount;
				itemInfo->amount = 0;
			}
		}
		break;

	// ������ ��Ŭ�� ���� ��
	case VK_RBUTTON:
		// �ڵ尡 �ִٸ�
		if (hand->amount)
		{
			// �ڵ忡�� �ϳ��� ��������
			EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(new ItemInfo(hand->id, 1));
			hand->amount -= 1;
		}
		// �ڵ尡 ���ٸ�
		else
		{
			// ���Կ� �������� �ִٸ� 
			if (itemInfo)
			{
				// ������ �ݸ� �ڵ��
				hand->id = itemInfo->id;
				hand->amount = itemInfo->amount / 2;
				itemInfo->amount -= itemInfo->amount / 2;
			}
		}
		break;
	}
}
// �� �ּ��� �����ּ���