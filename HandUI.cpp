#include "HandUI.h"
#include "InventorySlotUI.h"
#include "ItemOnGround.h"
#include "Character.h"
#include "Inventory.h"

HRESULT HandUI::Init()
{
	allItemImage = new GLImage();
	allItemImage->Init("Icons/AllItems", 8, 8, 0.25f, 0.25f, 512, 512);

	handItem = new ItemInfo();

	return S_OK;
}

void HandUI::Release()
{
	SAFE_DELETE(handItem);
	SAFE_DELETE(allItemImage);
}

void HandUI::Update()
{
	// 손 비우기 기능
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Q'))
	{
		EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(new ItemInfo(*handItem));
		handItem->amount = 0;
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown('Z'))
	{
		if (handItem->amount)
		{
			handItem->AddAmount(-1);

			ItemOnGround* item = new ItemOnGround();
			item->Init((ItemEnum)handItem->id);
			item->SetPosition(g_cursorPosition);
			EntityManager::GetSingleton()->AddItemOnGround(item);
		}
	}
}

void HandUI::Render(Shader* shader)
{
	if (handItem->amount)
	{
		if (!g_itemSpecs[handItem->id].buildable || UIManager::GetSingleton()->IsMouseOnUI())
		{
			glm::ivec2 maxFrame = allItemImage->GetMaxFrame();
			allItemImage->Render(shader, g_ptMouse.x + 16, g_ptMouse.y - 16, handItem->id % maxFrame.x, maxFrame.y - 1 - handItem->id / maxFrame.y);
		}

		TextRenderer::GetSingleton()->RenderText(to_string(handItem->amount), g_ptMouse.x + 16 - to_string(handItem->amount).length() * 6 + 17, g_ptMouse.y - 16 - 7.0f, 0.46f);
	}
}

bool HandUI::IsEmpty()
{
	return handItem->amount == 0;
}
