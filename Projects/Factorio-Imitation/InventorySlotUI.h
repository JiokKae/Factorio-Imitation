#pragma once
#include "SlotUI.h"

class InventorySlotUI : public SlotUI
{
	GLImage* handImage;

public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();

	virtual void OnClick(int key) override;
};
