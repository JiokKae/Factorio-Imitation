#pragma once
#include "UI.h"
#include "framework.h"

#define SLOT_X	10
#define SLOT_Y	9
#define SLOT_SIZE	(SLOT_X * SLOT_Y)

class Inventory;
class InventorySlotUI;
class DeactiveButtonUI;
class CharacterUI : public UI
{
	Inventory* inventory;
	InventorySlotUI* slotUI;
	DeactiveButtonUI* deactiveButtonUI;
public:
	virtual HRESULT Init(Inventory* inventory);
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(Shader* lpShader) override;
};