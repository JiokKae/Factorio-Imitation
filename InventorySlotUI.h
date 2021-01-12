#pragma once
#include "UI.h"

class InventorySlotUI : public UI
{
	ItemInfo* itemInfo;

	bool onMouse;
	bool isLMouseDown;
	bool isRMouseDown;

	GLImage* itemImage;

	GLImage* hand;
public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update(ItemInfo* itemInfo);
	virtual void Render(Shader* lpShader);

	void OnClick(int key) override;

	ItemInfo* GetLpItemSlot() { return itemInfo; }
};
