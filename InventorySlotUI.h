#pragma once
#include "SlotUI.h"

class InventorySlotUI : public SlotUI
{
	GLImage* hand;

public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();

	virtual void OnClick(int key) override;
};
