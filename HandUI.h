#pragma once
#include "UI.h"
class InventorySlotUI;
struct ItemInfo;
class HandUI : public UI
{
	ItemInfo* handItem;

	GLImage* itemImage;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(Shader* shader) override;

	ItemInfo* GetHandItem() { return handItem; }

	bool IsEmpty();
};

