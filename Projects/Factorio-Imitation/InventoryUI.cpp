#include "InventoryUI.h"
#include "InventorySlotUI.h"
#include "Inventory.h"
#include "Character.h"

HRESULT InventoryUI::Init()
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
	return S_OK;
}

void InventoryUI::Release()
{
	SAFE_ARR_RELEASE(slotUI, SLOT_SIZE);
}

void InventoryUI::Update()
{
	vector<ItemInfo*> arr = EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->GetItemInfoArray();
	for (int i = 0; i < arr.size(); i++)
	{
		slotUI[i].Update(arr[i]);
	}
	for (size_t i = arr.size(); i < SLOT_SIZE; i++)
	{
		slotUI[i].Update(nullptr);
	}
}

void InventoryUI::Render(ShaderProgram* lpShader)
{
	for (int i = 0; i < SLOT_SIZE; i++)
	{
		slotUI[i].Render(lpShader);
	}
	for (int i = 0; i < SLOT_SIZE; i++)
	{
		slotUI[i].LateRender(lpShader);
	}
}
