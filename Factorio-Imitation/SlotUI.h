#pragma once
#include "UI.h"

class SlotUI : public UI
{
protected:
	ItemInfo* itemInfo;

	bool onMouse;
	bool isLMouseDown;
	bool isRMouseDown;

	GLImage* allItemImage;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(ItemInfo* itemInfo);
	virtual void Render(Shader* shader) override;
	virtual void LateRender(Shader* shader);

	virtual void OnClick(int key) override;

	ItemInfo* GetLpItemSlot() { return itemInfo; }
};
