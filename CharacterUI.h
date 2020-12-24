#pragma once
#include "UI.h"
#include "framework.h"

#define SLOT_X	10
#define SLOT_Y	9
#define SLOT_SIZE	(SLOT_X * SLOT_Y)
class InventorySlotUI;
class CharacterUI : public UI
{
	InventorySlotUI* slotUI;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};