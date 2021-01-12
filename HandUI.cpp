#include "HandUI.h"
#include "InventorySlotUI.h"
#include "ItemOnGround.h"
#include "Character.h"
#include "Inventory.h"

HRESULT HandUI::Init()
{
	itemImage = new GLImage();
	itemImage->Init("Icons/Coal", 1, 1, 32, 32);

	handItem = new ItemInfo();

	return S_OK;
}

void HandUI::Release()
{
	SAFE_DELETE(handItem);
	SAFE_DELETE(itemImage);
}

void HandUI::Update()
{
	// 손 비우기 기능
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Q'))
	{
		EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(new ItemInfo(*handItem));
		handItem->amount = 0;
	}

	// 핸드아이템에 따라 바뀌는 이미지
	itemImage->SetSourceTexture(TextureManager::GetSingleton()->FindTexture("Icons/" + g_itemSpecs[handItem->id].name));

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
			itemImage->Render(shader, g_ptMouse.x + 16, g_ptMouse.y - 16);

		TextRenderer::GetSingleton()->RenderText(to_string(handItem->amount), g_ptMouse.x + 16 - to_string(handItem->amount).length() * 6 + 17, g_ptMouse.y - 16 - 7.0f, 0.46f);
	}
}

bool HandUI::IsEmpty()
{
	return handItem->amount == 0;
}
