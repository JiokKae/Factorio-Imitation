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
	// ���õ� ������ �ִٸ�
	if (this->selectedSlotUI)
	{
		// ������ ������ �����ߴٸ�
		if (this->selectedSlotUI == slotUI)
		{
			// ���õ� ���� ��������
			this->selectedSlotUI->SetIsSelected(false);
			selectedSlotUI = nullptr;
		}
		// ���ο� ������ �����ߴٸ�
		else
		{
			// ���õ� ���� ��ȯ
			this->selectedSlotUI->SetIsSelected(false);
			this->selectedSlotUI = slotUI;

			if (this->selectedSlotUI)
				this->selectedSlotUI->SetIsSelected(true);
		}
	}
	// ���õ� ������ ���ٸ�
	else
	{
		// ���ο� ���� ����
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
