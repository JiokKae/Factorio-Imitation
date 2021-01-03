#include "InventorySlotUI.h"
#include "GLImage.h"

HRESULT InventorySlotUI::Init(int x, int y)
{
	image = new GLImage();
	image->Init("UI/InventorySlotUI", 2, 1);
	localPosition.x = x * (GetWidth() + 2) -397;
	localPosition.y = y * (GetHeight() + 2) - 165;

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
		if (PtInFRect(GetFrect(), { g_ptMouse.x, g_ptMouse.y }))
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
