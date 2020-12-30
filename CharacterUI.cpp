#include "CharacterUI.h"
#include "InventorySlotUI.h"
#include "GLImage.h"
#include "Shader.h"

HRESULT CharacterUI::Init()
{
	image = new GLImage();
	image->Init("UI/CharacterUI");

	slotUI = new InventorySlotUI[SLOT_SIZE]();
	for (int y = 0; y < SLOT_Y; y++)
	{
		for (int x = 0; x < SLOT_X; x++)
		{
			slotUI[y * SLOT_X + x].Init(x, y);
			slotUI[y * SLOT_X + x].SetParent(this);
		}
	}

	return S_OK;
}

void CharacterUI::Release()
{
	for (int i = 0; i < SLOT_SIZE; i++)
	{
		slotUI[i].Release();
	}
	SAFE_ARR_DELETE(slotUI);
}

void CharacterUI::Update()
{
	if (active)
	{
		for (int i = 0; i < SLOT_SIZE; i++)
		{
			slotUI[i].Update();
		}
	}
}

void CharacterUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y);

		for (int i = 0; i < SLOT_SIZE; i++)
		{
			slotUI[i].Render(lpShader);
		}
	}
}
