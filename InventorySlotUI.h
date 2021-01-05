#pragma once
#include "UI.h"

class InventorySlotUI : public UI
{
	ItemInfo* itemInfo;

	bool onMouse;
	bool isMouseDown;
	bool isSelected;
	GLImage* itemImage;
	GLImage* hand;
public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update(ItemInfo* itemInfo);
	virtual void Render(Shader* lpShader);
	void HandRender(Shader* lpShader);

	void OnClick() override;

	void SetIsSelected(bool selected) { this->isSelected = selected; }

	ItemInfo* GetLpItemSlot() { return itemInfo; }
};
