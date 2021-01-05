#pragma once
#include "UI.h"
class InventorySlotUI;
class HandUI : public UI
{
	InventorySlotUI* currSlotUI;

public:
	void Update() override;
	void Render(Shader* shader) override;

	void SetCurrentSlotUI(InventorySlotUI* slotUI);
};

