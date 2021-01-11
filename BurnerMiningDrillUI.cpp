#include "BurnerMiningDrillUI.h"
#include "InventoryUI.h"
#include "DeactiveButtonUI.h"
#include "BurnerMiningDrill.h"

HRESULT BurnerMiningDrillUI::Init(Inventory* inventory)
{
	image = new GLImage();
	image->Init("UI/BunerMiningDrillUI");

	inventoryUI = new InventoryUI();
	inventoryUI->Init(inventory);
	inventoryUI->SetParent(this);
	inventoryUI->SetLocalPosition(glm::vec2(-180, 59));

	deactiveButtonUI = new DeactiveButtonUI();
	deactiveButtonUI->Init();
	deactiveButtonUI->SetParent(this);
	deactiveButtonUI->SetLocalPosition(glm::vec2(200, 286));

	redProgressiveBar = new GLImage();
	redProgressiveBar->Init("UI/RedProgressiveBarUI");
	redProgressiveBar->SetOffset(glm::vec2(redProgressiveBar->GetFrameWidth() / 2, 0.0f));

	greenProgressiveBar = new GLImage();
	greenProgressiveBar->Init("UI/GreenProgressiveBarUI");
	greenProgressiveBar->SetOffset(glm::vec2(greenProgressiveBar->GetFrameWidth() / 2, 0.0f));

	return S_OK;
}

void BurnerMiningDrillUI::Release()
{
	SAFE_RELEASE(image);
	SAFE_RELEASE(inventoryUI);
	SAFE_RELEASE(deactiveButtonUI);
	SAFE_RELEASE(redProgressiveBar);
	SAFE_RELEASE(greenProgressiveBar);
}

void BurnerMiningDrillUI::Update()
{
	if (active)
	{
		inventoryUI->Update();
		deactiveButtonUI->Update();
		redProgressiveBar->SetScale(glm::vec2(currBurnerMiningDrill->GetCurrPower() / currBurnerMiningDrill->GetMaxPower(), 1.0f));
		greenProgressiveBar->SetScale(glm::vec2(currBurnerMiningDrill->GetProductionPercent(), 1.0f));
		if (PtInFRect(GetFrect(), { g_ptMouse.x, g_ptMouse.y }))
		{
			KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON);
		}
	}
}

void BurnerMiningDrillUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y);
		inventoryUI->Render(lpShader);
		deactiveButtonUI->Render(lpShader);
		redProgressiveBar->Render(lpShader, GetPosition().x + 77 - redProgressiveBar->GetFrameWidth() / 2, GetPosition().y + 199);
		greenProgressiveBar->Render(lpShader, GetPosition().x + 37 - greenProgressiveBar->GetFrameWidth() / 2, GetPosition().y + 238);
	}
}
