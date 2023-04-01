#pragma once
#include "../GLFramework/UI/UI.h"

class Inventory;
class Recipe;
class SlotUI;
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
	SlotUI* resourceSlotUI;
	SlotUI* resultSlotUI;

	GLImage* redProgressiveBar;
	GLImage* greenProgressiveBar;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(ShaderProgram* lpShader) override;

	void SetCurrStoneFurnace(StoneFurnace* stoneFurnace) { this->currStoneFurnace = stoneFurnace; }
};

