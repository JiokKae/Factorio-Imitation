#pragma once
#include "framework.h"
#include "GLFramework/UI/UI.h"

class Inventory;
class InventoryUI;
class DeactiveButtonUI;
class CharacterUI : public UI
{
	InventoryUI* inventoryUI;
	DeactiveButtonUI* deactiveButtonUI;

public:
	virtual HRESULT Init();
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(Shader* lpShader) override;

};