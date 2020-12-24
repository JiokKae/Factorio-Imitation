#pragma once
#include "UI.h"
class InventorySlotUI : public UI
{
	bool onMouse;
public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};
