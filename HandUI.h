#pragma once
#include "UI.h"
class InventorySlotUI;
struct ItemInfo;
class HandUI : public UI
{
	InventorySlotUI* selectedSlotUI;

public:
	void Update() override;
	void Render(Shader* shader) override;

	void SelectSlotUI(InventorySlotUI* slotUI);

	ItemInfo* GetLpSelectedSlot();
};

