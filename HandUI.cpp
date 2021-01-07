#include "HandUI.h"
#include "InventorySlotUI.h"

void HandUI::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Q'))
	{
		SelectSlotUI(nullptr);
	}
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
	{
		if (selectedSlotUI)
		{
			ItemInfo* slot = selectedSlotUI->GetLpItemSlot();
			if (slot)
			{
				slot->AddAmount(-1);
				if (slot->amount == 0)
					SelectSlotUI(nullptr);
			}
		}
	}
}

void HandUI::Render(Shader* shader)
{
	if (selectedSlotUI)
		selectedSlotUI->HandRender(shader);
}

void HandUI::SelectSlotUI(InventorySlotUI* slotUI)
{
	// 선택된 슬롯이 있다면
	if (this->selectedSlotUI)
	{
		// 동일한 슬롯을 선택했다면
		if (this->selectedSlotUI == slotUI)
		{
			// 선택된 슬롯 선택해제
			this->selectedSlotUI->SetIsSelected(false);
			selectedSlotUI = nullptr;
		}
		// 새로운 슬롯을 선택했다면
		else
		{
			// 선택된 슬롯 전환
			this->selectedSlotUI->SetIsSelected(false);
			this->selectedSlotUI = slotUI;

			if (this->selectedSlotUI)
				this->selectedSlotUI->SetIsSelected(true);
		}
	}
	// 선택된 슬롯이 없다면
	else
	{
		// 새로운 슬롯 선택
		this->selectedSlotUI = slotUI;

		if(this->selectedSlotUI)
			this->selectedSlotUI->SetIsSelected(true);
	}
}

ItemInfo* HandUI::GetLpSelectedSlot()
{
	if (selectedSlotUI)
		return selectedSlotUI->GetLpItemSlot();
	else
		return nullptr;
}
