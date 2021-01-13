#include "InventorySlotUI.h"
#include "GLImage.h"
#include "TextRenderer.h"
#include "HandUI.h"
#include "Character.h"
#include "Inventory.h"
HRESULT InventorySlotUI::Init(int x, int y)
{
	image = new GLImage();
	image->Init("UI/SlotUI", 3, 1);
	
	allItemImage = new GLImage();
	allItemImage->Init("Icons/AllItems", 8, 8, 0.25f, 0.25f, 512, 512);

	hand = new GLImage();
	hand->Init("Icons/Hand", 1, 1, 32, 32);

	localPosition.x = x * (GetWidth() + 2);
	localPosition.y = -y * (GetHeight() + 2);

	onMouse = false;
	isLMouseDown = false;
	isRMouseDown = false;
	return S_OK;
}

void InventorySlotUI::Release()
{
}

void InventorySlotUI::Update(ItemInfo* itemInfo)
{
	if (active)
	{
		this->itemInfo = itemInfo;

		if (PtInFRect(GetFrect(), { g_ptMouse.x, g_ptMouse.y }))
		{
			onMouse = true;
			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
			{
				isLMouseDown = true;
				if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
					OnClick(VK_LBUTTON);
			}
			else
				isLMouseDown = false;

			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RBUTTON))
			{
				isRMouseDown = true;
				if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
					OnClick(VK_RBUTTON);
			}
			else
				isRMouseDown = false;
		}
		else
		{
			onMouse = false;
			isLMouseDown = false;
			isRMouseDown = false;
		}
	}
}

void InventorySlotUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y, onMouse + (isLMouseDown || isRMouseDown), 0);
		if (itemInfo && itemInfo->amount != 0)
		{
			glm::ivec2 maxFrame = allItemImage->GetMaxFrame();
			allItemImage->Render(lpShader, GetPosition().x, GetPosition().y, itemInfo->id % maxFrame.x, maxFrame.y - 1 - itemInfo->id / maxFrame.y);
			TextRenderer::GetSingleton()->RenderText(to_string(itemInfo->amount), GetPosition().x - to_string(itemInfo->amount).length() * 6 + 17, GetPosition().y - 7.0f, 0.46f);
			
		}
	}
}

void InventorySlotUI::LateRender(Shader* shader)
{
	if (itemInfo && itemInfo->amount != 0)
	{
		if (onMouse)
			TextRenderer::GetSingleton()->RenderText(g_itemSpecs[itemInfo->id].name, g_ptMouse.x + 20, g_ptMouse.y - 20, 1.0f, glm::vec3(1.0f));
	}
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