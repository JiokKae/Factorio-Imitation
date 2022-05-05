#include "StoneFurnaceUI.h"
#include "InventoryUI.h"
#include "DeactiveButtonUI.h"
#include "StoneFurnace.h"
#include "FuelSlotUI.h"
#include "SlotUI.h"

HRESULT StoneFurnaceUI::Init()
{
	image = new GLImage();
	image->Init("UI/StoneFurnaceUI");

	inventoryUI = new InventoryUI();
	inventoryUI->Init();
	inventoryUI->SetParent(this);
	inventoryUI->SetLocalPosition(glm::vec2(-180, 53));

	deactiveButtonUI = new DeactiveButtonUI();
	deactiveButtonUI->Init();
	deactiveButtonUI->SetParent(this);
	deactiveButtonUI->SetLocalPosition(glm::vec2(200, 290));

	redProgressiveBar = new GLImage();
	redProgressiveBar->Init("UI/RedProgressiveBarUI");
	redProgressiveBar->SetOffset(glm::vec2(redProgressiveBar->GetFrameWidth() / 2, 0.0f));

	greenProgressiveBar = new GLImage();
	greenProgressiveBar->Init("UI/GreenProgressiveBarUI");
	greenProgressiveBar->SetOffset(glm::vec2(greenProgressiveBar->GetFrameWidth() / 2, 0.0f));

	// slots init
	fuelSlotUI = new FuelSlotUI();
	fuelSlotUI->Init();
	fuelSlotUI->SetParent(this);
	fuelSlotUI->SetLocalPosition(glm::vec2(-72, 170));

	resourceSlotUI = new SlotUI();
	resourceSlotUI->Init();
	resourceSlotUI->SetParent(this);
	resourceSlotUI->SetLocalPosition(glm::vec2(-72, 237));

	resultSlotUI = new SlotUI();
	resultSlotUI->Init();
	resultSlotUI->SetParent(this);
	resultSlotUI->SetLocalPosition(glm::vec2(181, 237));

	return S_OK;
}

void StoneFurnaceUI::Release()
{
	SAFE_RELEASE(greenProgressiveBar);
	SAFE_RELEASE(redProgressiveBar);
	SAFE_RELEASE(deactiveButtonUI);
	SAFE_RELEASE(inventoryUI);
	SAFE_RELEASE(image);

	// slots release
	SAFE_RELEASE(fuelSlotUI);
	SAFE_RELEASE(resourceSlotUI);
	SAFE_RELEASE(resultSlotUI);
}

void StoneFurnaceUI::Update()
{
	if (active)
	{
		inventoryUI->Update();
		deactiveButtonUI->Update();
		redProgressiveBar->SetScale(glm::vec2(currStoneFurnace->GetCurrPower() / currStoneFurnace->GetMaxPower(), 1.0f));
		greenProgressiveBar->SetScale(glm::vec2(currStoneFurnace->GetProductionPercent() * 0.81f, 1.0f));
		
		// slots update
		fuelSlotUI->Update(currStoneFurnace->GetFuel());
		resourceSlotUI->Update(currStoneFurnace->GetResource());
		resultSlotUI->Update(currStoneFurnace->GetResult());

		if (PtInFRect(GetFrect(), { g_ptMouse.x, g_ptMouse.y }))
		{
			KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON);
		}
	}
}

void StoneFurnaceUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y);
		inventoryUI->Render(lpShader);
		deactiveButtonUI->Render(lpShader);
		redProgressiveBar->Render(lpShader, GetPosition().x + 76 - redProgressiveBar->GetFrameWidth() / 2, GetPosition().y + 170);
		greenProgressiveBar->Render(lpShader, GetPosition().x + 80 - greenProgressiveBar->GetFrameWidth() / 2, GetPosition().y + 238);
		fuelSlotUI->Render(lpShader);
		resourceSlotUI->Render(lpShader);
		resultSlotUI->Render(lpShader);
	}
}
