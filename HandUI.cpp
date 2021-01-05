#include "HandUI.h"
#include "InventorySlotUI.h"

void HandUI::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Q'))
	{
		SetCurrentSlotUI(nullptr);
	}
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
	{
		if (currSlotUI)
		{
			currSlotUI->GetLpItemSlot()->AddAmount(-1);
			if (currSlotUI->GetLpItemSlot()->amount == 0)
				SetCurrentSlotUI(nullptr);
		}
	}
}

void HandUI::Render(Shader* shader)
{
	if (currSlotUI)
		currSlotUI->HandRender(shader);
}

void HandUI::SetCurrentSlotUI(InventorySlotUI* slotUI)
{
	if(currSlotUI)
		this->currSlotUI->SetIsSelected(false);
	this->currSlotUI = slotUI;
}
