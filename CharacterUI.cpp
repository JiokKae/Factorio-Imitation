#include "CharacterUI.h"
#include "InventoryUI.h"
#include "DeactiveButtonUI.h"

HRESULT CharacterUI::Init(Inventory* inventory)
{
	image = new GLImage();
	image->Init("UI/CharacterUI");
	
	inventoryUI = new InventoryUI();
	inventoryUI->Init(inventory);
	inventoryUI->SetParent(this);
	inventoryUI->SetLocalPosition(glm::vec2(-397.5, 153));

	deactiveButtonUI = new DeactiveButtonUI();
	deactiveButtonUI->Init();
	deactiveButtonUI->SetParent(this);
	deactiveButtonUI->SetLocalPosition(glm::vec2(419, 231));

	return S_OK;
}

void CharacterUI::Release()
{
	SAFE_RELEASE(image);
	SAFE_RELEASE(inventoryUI);
	SAFE_RELEASE(deactiveButtonUI);
}

void CharacterUI::Update()
{
	if (active)
	{
		inventoryUI->Update();
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

		inventoryUI->Render(lpShader);
		deactiveButtonUI->Render(lpShader);
	}
}
