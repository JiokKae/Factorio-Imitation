#include "BurnerMiningDrillUI.h"
#include "InventoryUI.h"
#include "DeactiveButtonUI.h"
#include "BurnerMiningDrill.h"
#include "FuelSlotUI.h"

HRESULT BurnerMiningDrillUI::Init()
{
	image = new GLImage("UI/BunerMiningDrillUI");

	inventoryUI = new InventoryUI();
	inventoryUI->Init();
	inventoryUI->SetParent(this);
	inventoryUI->SetLocalPosition(glm::vec2(-180, 59));

	deactiveButtonUI = new DeactiveButtonUI();
	deactiveButtonUI->Init();
	deactiveButtonUI->SetParent(this);
	deactiveButtonUI->SetLocalPosition(glm::vec2(200, 286));

	redProgressiveBar = new GLImage("UI/RedProgressiveBarUI");
	redProgressiveBar->SetOffset(glm::vec2(redProgressiveBar->GetFrameWidth() / 2, 0.0f));

	greenProgressiveBar = new GLImage("UI/GreenProgressiveBarUI");
	greenProgressiveBar->SetOffset(glm::vec2(greenProgressiveBar->GetFrameWidth() / 2, 0.0f));

	fuelSlotUI = new FuelSlotUI();
	fuelSlotUI->Init();
	fuelSlotUI->SetParent(this);
	fuelSlotUI->SetLocalPosition(glm::vec2(-72, 199));

	return S_OK;
}

void BurnerMiningDrillUI::Release()
{
	SAFE_RELEASE(fuelSlotUI);
	SAFE_DELETE(greenProgressiveBar);
	SAFE_DELETE(redProgressiveBar);
	SAFE_RELEASE(deactiveButtonUI);
	SAFE_RELEASE(inventoryUI);
	SAFE_DELETE(image);
}

void BurnerMiningDrillUI::Update()
{
	if (active)
	{
		inventoryUI->Update();
		deactiveButtonUI->Update();
		redProgressiveBar->SetScale(glm::vec2(currBurnerMiningDrill->GetCurrPower() / currBurnerMiningDrill->GetMaxPower(), 1.0f));
		greenProgressiveBar->SetScale(glm::vec2(currBurnerMiningDrill->GetProductionPercent(), 1.0f));
		fuelSlotUI->Update(currBurnerMiningDrill->GetWaitingItemInfo());
	}
}

void BurnerMiningDrillUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y);
		inventoryUI->Render(lpShader);
		deactiveButtonUI->Render(lpShader);
		redProgressiveBar->Render(lpShader, GetPosition().x + 77 - redProgressiveBar->GetFrameWidth() / 2, GetPosition().y + 198.5f);
		greenProgressiveBar->Render(lpShader, GetPosition().x + 37 - greenProgressiveBar->GetFrameWidth() / 2, GetPosition().y + 238.0f);
		fuelSlotUI->Render(lpShader);
	}
}
