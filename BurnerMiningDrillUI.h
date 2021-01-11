#pragma once
#include "UI.h"
#include "framework.h"

class Inventory;
class InventoryUI;
class DeactiveButtonUI;
class BurnerMiningDrill;
class BurnerMiningDrillUI : public UI
{
	InventoryUI* inventoryUI;
	DeactiveButtonUI* deactiveButtonUI;
	BurnerMiningDrill* currBurnerMiningDrill;

	GLImage* redProgressiveBar;
	GLImage* greenProgressiveBar;
public:
	virtual HRESULT Init(Inventory* inventory);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	void SetCurrBurnerMiningDrill(BurnerMiningDrill* burnerMiningDrill) { this->currBurnerMiningDrill = burnerMiningDrill; }
};

