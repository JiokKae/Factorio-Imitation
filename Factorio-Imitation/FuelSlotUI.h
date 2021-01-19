#pragma once
#include "SlotUI.h"
class FuelSlotUI : public SlotUI
{
	GLImage* slotImage;
	GLImage* allItemImage;

	ItemInfo* itemInfo;

	bool onMouse;
	bool isLMouseDown;
	bool isRMouseDown;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(ItemInfo* itemInfo);
	virtual void Render(Shader* shader) override;

	virtual void OnClick(int key) override;
};

