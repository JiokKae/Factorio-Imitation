#include "InventorySlotUI.h"
#include "GLImage.h"
#include "TextRenderer.h"
#include "HandUI.h"

HRESULT InventorySlotUI::Init(int x, int y)
{
	image = new GLImage();
	image->Init("UI/InventorySlotUI", 2, 1);
	
	itemImage = new GLImage();
	itemImage->Init("Icons/Coal", 1, 1, 32, 32);

	hand = new GLImage();
	hand->Init("Icons/Hand", 1, 1, 32, 32);

	localPosition.x = x * (GetWidth() + 2);
	localPosition.y = -y * (GetHeight() + 2);

	onMouse = false;
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

		if (itemInfo)
			itemImage->SetSourceTexture(TextureManager::GetSingleton()->FindTexture("Icons/" + g_itemSpecs[itemInfo->id].name ));

		if (PtInFRect(GetFrect(), { g_ptMouse.x, g_ptMouse.y }))
		{
			onMouse = true;
			if (isMouseDown)
			{
				if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
					OnClick();
			}

			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
			{
				isMouseDown = true;
			}
			else
				isMouseDown = false;
		}
		else
		{
			onMouse = false;
		}
	}
}

void InventorySlotUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y, onMouse, 0);
		if (itemInfo && itemInfo->amount != 0)
		{
			if (isSelected)
			{
				hand->Render(lpShader, GetPosition().x, GetPosition().y);
			}
			else
			{
				itemImage->Render(lpShader, GetPosition().x, GetPosition().y);
				TextRenderer::GetSingleton()->RenderText(to_string(itemInfo->amount), GetPosition().x - to_string(itemInfo->amount).length() * 6 + 17, GetPosition().y - 7.0f, 0.46f);
			}
		}
		
	}
}
void InventorySlotUI::HandRender(Shader* lpShader)
{
	if (isSelected)
	{
		if (itemInfo)
		{
			if (g_itemSpecs[itemInfo->id].buildable && !UIManager::GetSingleton()->IsMouseOnUI())
			{
				TextRenderer::GetSingleton()->RenderText(to_string(itemInfo->amount), g_ptMouse.x + 16 - to_string(itemInfo->amount).length() * 6 + 17, g_ptMouse.y - 16 - 7.0f, 0.46f);
			}
			else
			{
				itemImage->Render(lpShader, g_ptMouse.x + 16, g_ptMouse.y - 16);
				TextRenderer::GetSingleton()->RenderText(to_string(itemInfo->amount), g_ptMouse.x + 16 - to_string(itemInfo->amount).length() * 6 + 17, g_ptMouse.y - 16 - 7.0f, 0.46f);
			}
		}
	}
}

void InventorySlotUI::OnClick()
{
	UIManager::GetSingleton()->GetLpHandUI()->SelectSlotUI(this);
}
// 이 주석을 지워주세요