#pragma once
#include "UI.h"

class Inventory;
class Recipe;
class FuelSlotUI;
class InventoryUI;
class DeactiveButtonUI;
class StoneFurnace;
class StoneFurnaceUI : public UI
{
	InventoryUI* inventoryUI;
	DeactiveButtonUI* deactiveButtonUI;
	StoneFurnace* currStoneFurnace;

	FuelSlotUI* fuelSlotUI;
	GLImage* redProgressiveBar;
	GLImage* greenProgressiveBar;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(Shader* lpShader) override;

	void SetCurrStoneFurnace(StoneFurnace* burnerMiningDrill) { this->currStoneFurnace = burnerMiningDrill; }
};

