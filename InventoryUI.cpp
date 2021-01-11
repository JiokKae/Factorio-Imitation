#include "InventoryUI.h"
#include "InventorySlotUI.h"
#include "Inventory.h"

HRESULT InventoryUI::Init(Inventory* inventory)
{
	slotUI = new InventorySlotUI[SLOT_SIZE]();
	for (int y = 0; y < SLOT_Y; y++)
	{
		for (int x = 0; x < SLOT_X; x++)
		{
			slotUI[y * SLOT_X + x].Init(x, y);
			slotUI[y * SLOT_X + x].SetParent(this);
		}
	}

	this->inventory = inventory;

	return S_OK;
}

void InventoryUI::Release()
{
	for (int i = 0; i < SLOT_SIZE; i++)
	{
		slotUI[i].Release();
	}
	SAFE_ARR_DELETE(slotUI);
}

void InventoryUI::Update()
{
	vector<ItemInfo*> arr = inventory->GetItemInfoArray();
	for (int i = 0; i < arr.size(); i++)
	{
		slotUI[i].Update(arr[i]);
	}
	for (int i = arr.size(); i < SLOT_SIZE; i++)
	{
		slotUI[i].Update(nullptr);
	}
}

void InventoryUI::Render(Shader* lpShader)
{
	for (int i = 0; i < SLOT_SIZE; i++)
	{
		slotUI[i].Render(lpShader);
	}
}
