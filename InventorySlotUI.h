#pragma once
#include "UI.h"

class InventorySlotUI : public UI
{
	string itemName;
	int itemAmount;

	bool onMouse;
	bool isMouseDown;
	bool isSelected;
	GLImage* image2;
	GLImage* hand;
public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update(string itemName = "", int itemAmount = 0);
	virtual void Render(Shader* lpShader);
};
