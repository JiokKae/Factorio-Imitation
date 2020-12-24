#include "InventorySlotUI.h"
#include "GLImage.h"

HRESULT InventorySlotUI::Init(int x, int y)
{
	image = new GLImage();
	image->Init("UI/InventorySlotUI", 76, 38, 2, 1);
	width = image->GetFrameWidth();
	height = image->GetFrameHeight();
	localPosition.x = x * (width + 2) -397;
	localPosition.y = y * (height + 2) - 165;

	onMouse = false;
	return S_OK;
}

void InventorySlotUI::Release()
{
}

void InventorySlotUI::Update()
{
	if (active)
	{
		if (g_ptMouse.x <= width / 2 + GetPosition().x && g_ptMouse.x >= GetPosition().x - width / 2 &&
			g_ptMouse.y <= height / 2 + GetPosition().y && g_ptMouse.y >= GetPosition().y - height / 2)
		{
			onMouse = true;
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
	}
}
