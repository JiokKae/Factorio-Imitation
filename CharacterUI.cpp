#include "CharacterUI.h"
#include "InventorySlotUI.h"
#include "Inventory.h"
#include "DeactiveButtonUI.h"
#include "GLImage.h"
#include "Shader.h"

HRESULT CharacterUI::Init(Inventory* inventory)
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

	this->inventory = inventory;

	deactiveButtonUI = new DeactiveButtonUI();
	deactiveButtonUI->Init();
	deactiveButtonUI->SetParent(this);
	deactiveButtonUI->SetLocalPosition({ 419, 231 });

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
		vector<ItemInfo> arr = inventory->GetItemInfoArray();
		for (int i = 0; i < arr.size(); i++)
		{
			slotUI[i].Update(arr[i].name, arr[i].amount);
		}
		for (int i = arr.size(); i < SLOT_SIZE; i++)
		{
			slotUI[i].Update();
		}
		deactiveButtonUI->Update();
		if (PtInFRect(GetFrect(), { g_ptMouse.x, g_ptMouse.y }))
		{
			KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON);
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
		for (int i = 0; i < SLOT_SIZE; i++)
		{
			
		}
		
		deactiveButtonUI->Render(lpShader);
	}
}
