#include "FuelSlotUI.h"
#include "HandUI.h"

HRESULT FuelSlotUI::Init()
{
	image = new GLImage();
	image->Init("UI/FuelSlotUI", 3, 1);
	
	slotImage = new GLImage();
	slotImage->Init("UI/SlotUI", 3, 1);

	itemInfo = new ItemInfo(0, 0);

	return S_OK;
}

void FuelSlotUI::Release()
{
	SAFE_DELETE(itemInfo);
	SAFE_RELEASE(allItemImage);
	SAFE_RELEASE(slotImage);
	SAFE_RELEASE(image);
}

void FuelSlotUI::Render(Shader* shader)
{
	if (active)
	{
		if (itemInfo && itemInfo->amount != 0)
		{
			slotImage->Render(shader, GetPosition().x, GetPosition().y, onMouse + (isLMouseDown || isRMouseDown), 0);
			glm::ivec2 maxFrame = allItemImage->GetMaxFrame();
			allItemImage->Render(shader, GetPosition().x, GetPosition().y, itemInfo->id % maxFrame.x, maxFrame.y - 1 - itemInfo->id / maxFrame.y);
			TextRenderer::GetSingleton()->RenderText(to_string(itemInfo->amount), GetPosition().x - to_string(itemInfo->amount).length() * 6 + 17, GetPosition().y - 7.0f, 0.46f);
		}
		else
		{
			image->Render(shader, GetPosition().x, GetPosition().y, onMouse + (isLMouseDown || isRMouseDown), 0);
		}
	}
}

void FuelSlotUI::OnClick(int key)
{
	ItemInfo* hand = UIManager::GetSingleton()->GetLpHandUI()->GetHandItem();
	switch (key)
	{
	//������ ��Ŭ�� ���� ��
	case VK_LBUTTON:
		//�ڵ尡 �ִٸ�
		if (hand->amount)
		{
			// �ڵ尡 ������
			if (g_itemSpecs[hand->id].fuel)
			{
				// ���Կ� �������� �ִٸ�
				if (itemInfo->amount)
				{
					// ���԰� ���� ������
					if (hand->id == itemInfo->id)
					{
						// �ڵ带 ���� ��������
						itemInfo->amount += hand->amount;
						hand->amount = 0;
					}
					// ���԰� �ٸ� ������
					else
					{
						// �ڵ�� ������ ����
						ItemInfo tempInfo(itemInfo->id, itemInfo->amount);
						itemInfo->id = hand->id;
						itemInfo->amount = hand->amount;
						hand->id = tempInfo.id;
						hand->amount = tempInfo.amount;
					}
				}
				// ���Կ� �������� ���ٸ�
				else
				{
					// �ڵ带 ���� ��������
					itemInfo->id = hand->id;
					itemInfo->amount += hand->amount;
					hand->amount = 0;
				}
			}
		}
		// �ڵ尡 ���ٸ�
		else
		{
			// ������ ���� �ڵ��
			hand->id = itemInfo->id;
			hand->amount = itemInfo->amount;
			itemInfo->amount = 0;
		}
		break;

	// ������ ��Ŭ�� ���� ��
	case VK_RBUTTON:
		// �ڵ尡 �ְ�
		if (hand->amount)
		{
			// �ڵ尡 ������
			if (g_itemSpecs[hand->id].fuel)
			{
				// ���Կ� ���ᰡ �ִٸ�
				if (itemInfo->amount)
				{
					// ���԰� ���� ������
					if (hand->id == itemInfo->id)
					{
						// �ڵ忡�� �ϳ��� ��������
						itemInfo->amount += 1;
						hand->amount -= 1;
					}
				}
				// ���Կ� ���ᰡ ���ٸ�
				else
				{
					// �ڵ忡�� �ϳ��� ��������
					itemInfo->id = hand->id;
					itemInfo->amount += 1;
					hand->amount -= 1;
				}
			}
		}
		// �ڵ尡 ���ٸ�
		else
		{
			// ������ �ݸ� �ڵ��
			hand->id = itemInfo->id;
			hand->amount = itemInfo->amount / 2;
			itemInfo->amount -= itemInfo->amount / 2;
		}
		break;
	}
}