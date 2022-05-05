#pragma once
#include "SlotUI.h"
class FuelSlotUI : public SlotUI
{
	GLImage* fuelImage;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Render(Shader* shader) override;

	virtual void OnClick(int key) override;
};

