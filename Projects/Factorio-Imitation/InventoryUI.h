#pragma once
#include "../GLFramework/UI/UI.h"
#include "framework.h"

class Inventory;
class InventorySlotUI;
class InventoryUI : public UI
{
	Inventory* inventory;
	InventorySlotUI* slotUI;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(ShaderProgram* lpShader) override;

private:
	static constexpr int SLOT_ROW = 10;
	static constexpr int SLOT_COL = 9;
	static constexpr int SLOT_SIZE = SLOT_ROW * SLOT_COL;
};
