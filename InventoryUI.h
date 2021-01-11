#pragma once
#include "UI.h"
#include "framework.h"

class Inventory;
class InventorySlotUI;
class InventoryUI : public UI
{
	Inventory* inventory;
	InventorySlotUI* slotUI;

public:
	virtual HRESULT Init(Inventory* inventory);
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(Shader* lpShader) override;
};
