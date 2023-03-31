#include "SlotUI.h"
#include "../GLFramework/Image/GLImage.h"
#include "../GLFramework/TextRenderer/TextRenderer.h"
#include "HandUI.h"
#include "Character.h"
#include "Inventory.h"

HRESULT SlotUI::Init()
{
	image = new GLImage("UI/SlotUI", 3, 1);

	allItemImage = new GLImage("Icons/AllItems", 8, 8, 0.25f, 0.25f);
	allItemImage->SetScale(glm::vec2(0.25f));

	onMouse = false;
	isLMouseDown = false;
	isRMouseDown = false;

	return S_OK;
}

void SlotUI::Release()
{
	SAFE_DELETE(image);
	SAFE_DELETE(allItemImage);
}

void SlotUI::Update(ItemInfo* _itemInfo)
{
	if (active)
	{
		this->itemInfo = _itemInfo;

		if (PtInFRect(GetFrect(), { g_ptMouse.x, g_ptMouse.y }))
		{
			onMouse = true;
			if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
				OnClick(VK_LBUTTON);

			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
			{
				isLMouseDown = true;
			}
			else
				isLMouseDown = false;

			if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
				OnClick(VK_RBUTTON);

			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RBUTTON))
			{
				isRMouseDown = true;

			}
			else
				isRMouseDown = false;
		}
		else
		{
			onMouse = false;
			isLMouseDown = false;
			isRMouseDown = false;
		}
	}
}

void SlotUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y, onMouse + (isLMouseDown || isRMouseDown), 0);
		if (itemInfo && itemInfo->amount != 0)
		{
			glm::ivec2 maxFrame = allItemImage->GetMaxFrame();
			allItemImage->Render(lpShader, GetPosition().x, GetPosition().y, itemInfo->id % maxFrame.x, maxFrame.y - 1 - itemInfo->id / maxFrame.y);
			TextRenderer::GetSingleton()->RenderText(to_string(itemInfo->amount), GetPosition().x - to_string(itemInfo->amount).length() * 6 + 17, GetPosition().y - 7.0f, 0.46f);

		}
	}
}

void SlotUI::LateRender(Shader* /*shader*/)
{
	if (itemInfo && itemInfo->amount != 0)
	{
		if (onMouse)
			TextRenderer::GetSingleton()->RenderText(g_itemSpecs[itemInfo->id].name, g_ptMouse.x + 20.0f, g_ptMouse.y - 20.0f, 1.0f, glm::vec3(1.0f));
	}
}

void SlotUI::OnClick(int key)
{
	ItemInfo* hand = UIManager::GetSingleton()->GetLpHandUI()->GetHandItem();
	switch (key)
	{
	//슬롯을 좌클릭 했을 때
	case VK_LBUTTON:
		// 핸드가 비어 있을 때 : 핸드로 회수
		if (hand->IsEmpty())
			this->itemInfo->MoveAllItemTo(hand);

		else if (this->itemInfo->CanInput(hand->id))
		{
			// 슬롯이 비어 있을 때 : 핸드에서 슬롯으로
			if (this->itemInfo->IsEmpty())
				hand->MoveAllItemTo(this->itemInfo);

			// 핸드와 슬롯이 같은 종류일 때 : 핸드에서 슬롯으로
			else if (hand->id == this->itemInfo->id)
				hand->MoveAllItemTo(this->itemInfo);

			// 핸드와 슬롯이 다른 종류일 때 : 핸드와 슬롯을 스왑
			else
				this->itemInfo->SwapItemWith(itemInfo);
		}
		break;

	// 슬롯을 우클릭 했을 때
	case VK_RBUTTON:
		// 핸드가 있다면
		if (hand->amount)
		{
			// 슬롯에 아이템이 있고 슬롯과 핸드가 같은 아이템 일때만
			if (this->itemInfo->amount && this->itemInfo->id == hand->id)
			{
				this->itemInfo->amount += 1;
				hand->amount -= 1;
			}
		}
		// 핸드가 없다면
		else
		{
			// 슬롯에 아이템이 있다면 
			if (itemInfo->amount)
			{
				// 슬롯의 반만 핸드로
				hand->id = itemInfo->id;
				hand->amount = itemInfo->amount / 2;
				itemInfo->amount -= itemInfo->amount / 2;
			}
		}
		break;
	}
}
// 이 주석을 지워주세요