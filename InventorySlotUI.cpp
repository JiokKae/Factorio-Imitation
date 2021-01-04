#include "InventorySlotUI.h"
#include "GLImage.h"
#include "TextRenderer.h"
HRESULT InventorySlotUI::Init(int x, int y)
{
	image = new GLImage();
	image->Init("UI/InventorySlotUI", 2, 1);
	
	image2 = new GLImage();
	image2->Init("Icons/Coal", 1, 1, 32, 32);

	hand = new GLImage();
	hand->Init("Icons/Hand", 1, 1, 32, 32);

	localPosition.x = x * (GetWidth() + 2) -397.5;
	localPosition.y = -y * (GetHeight() + 2) + 153;

	onMouse = false;
	return S_OK;
}

void InventorySlotUI::Release()
{
}

void InventorySlotUI::Update(string itemName, int itemAmount)
{
	if (active)
	{
		this->itemName = itemName;
		this->itemAmount = itemAmount;

		if (PtInFRect(GetFrect(), { g_ptMouse.x, g_ptMouse.y }))
		{
			onMouse = true;
			if (isMouseDown)
			{
				if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
				{
					isSelected = true;
				}
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
	if (isSelected)
	{
		image2->Render(lpShader, g_ptMouse.x + 16, g_ptMouse.y - 16);
		TextRenderer::GetSingleton()->RenderText("100", g_ptMouse.x + 16, g_ptMouse.y - 16 - 7.0f, 0.46f);
	}

	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y, onMouse, 0);
		if (itemAmount != 0)
		{
			if (isSelected)
			{
				hand->Render(lpShader, GetPosition().x, GetPosition().y);
			}
			else
			{
				image2->Render(lpShader, GetPosition().x, GetPosition().y);
				TextRenderer::GetSingleton()->RenderText(to_string(itemAmount), GetPosition().x, GetPosition().y - 7.0f, 0.46f);
			}
		}
		
	}
}
// 이 주석을 지워주세요