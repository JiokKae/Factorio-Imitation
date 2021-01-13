#pragma once
#include "UI.h"

class InventorySlotUI : public UI
{
	ItemInfo* itemInfo;

	bool onMouse;
	bool isLMouseDown;
	bool isRMouseDown;

	GLImage* allItemImage;

	GLImage* hand;
public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update(ItemInfo* itemInfo);
	virtual void Render(Shader* shader);
	virtual void LateRender(Shader* shader);

	void OnClick(int key) override;

	ItemInfo* GetLpItemSlot() { return itemInfo; }
};
