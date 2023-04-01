#pragma once
#include "../GLFramework/UI/UI.h"
#include "framework.h"

class Inventory;
class FuelSlotUI;
class InventoryUI;
class DeactiveButtonUI;
class BurnerMiningDrill;
class BurnerMiningDrillUI : public UI
{
	InventoryUI* inventoryUI;
	DeactiveButtonUI* deactiveButtonUI;
	BurnerMiningDrill* currBurnerMiningDrill;

	FuelSlotUI* fuelSlotUI;
	GLImage* redProgressiveBar;
	GLImage* greenProgressiveBar;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(ShaderProgram* lpShader) override;

	void SetCurrBurnerMiningDrill(BurnerMiningDrill* burnerMiningDrill) { this->currBurnerMiningDrill = burnerMiningDrill; }
};

