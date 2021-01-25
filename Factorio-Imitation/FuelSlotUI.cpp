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
	//슬롯을 좌클릭 했을 때
	case VK_LBUTTON:
		//핸드가 있다면
		if (hand->amount)
		{
			// 핸드가 연료라면
			if (g_itemSpecs[hand->id].fuel)
			{
				// 슬롯에 아이템이 있다면
				if (itemInfo->amount)
				{
					// 슬롯과 같은 연료라면
					if (hand->id == itemInfo->id)
					{
						// 핸드를 전부 슬롯으로
						itemInfo->amount += hand->amount;
						hand->amount = 0;
					}
					// 슬롯과 다른 연료라면
					else
					{
						// 핸드와 슬롯을 스왑
						ItemInfo tempInfo(itemInfo->id, itemInfo->amount);
						itemInfo->id = hand->id;
						itemInfo->amount = hand->amount;
						hand->id = tempInfo.id;
						hand->amount = tempInfo.amount;
					}
				}
				// 슬롯에 아이템이 없다면
				else
				{
					// 핸드를 전부 슬롯으로
					itemInfo->id = hand->id;
					itemInfo->amount += hand->amount;
					hand->amount = 0;
				}
			}
		}
		// 핸드가 없다면
		else
		{
			// 아이템 전부 핸드로
			hand->id = itemInfo->id;
			hand->amount = itemInfo->amount;
			itemInfo->amount = 0;
		}
		break;

	// 슬롯을 우클릭 했을 때
	case VK_RBUTTON:
		// 핸드가 있고
		if (hand->amount)
		{
			// 핸드가 연료라면
			if (g_itemSpecs[hand->id].fuel)
			{
				// 슬롯에 연료가 있다면
				if (itemInfo->amount)
				{
					// 슬롯과 같은 연료라면
					if (hand->id == itemInfo->id)
					{
						// 핸드에서 하나만 슬롯으로
						itemInfo->amount += 1;
						hand->amount -= 1;
					}
				}
				// 슬롯에 연료가 없다면
				else
				{
					// 핸드에서 하나만 슬롯으로
					itemInfo->id = hand->id;
					itemInfo->amount += 1;
					hand->amount -= 1;
				}
			}
		}
		// 핸드가 없다면
		else
		{
			// 슬롯의 반만 핸드로
			hand->id = itemInfo->id;
			hand->amount = itemInfo->amount / 2;
			itemInfo->amount -= itemInfo->amount / 2;
		}
		break;
	}
}