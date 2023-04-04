#include "HandUI.h"
#include "InventorySlotUI.h"
#include "ItemOnGround.h"
#include "Character.h"
#include "Inventory.h"
#include "Structure.h"
#include "Tile.h"

HRESULT HandUI::Init()
{
	allItemImage = new GLImage("Icons/AllItems", 8, 8, 0.25f, 0.25f);
	allItemImage->SetScale(glm::vec2(0.25f));

	background = new GLImage("UI/BackgroundUI");
	background->SetAlpha(0.7f);

	backgroundName = new GLImage("UI/BackgroundNameUI");;

	dismantleBar = new GLImage("UI/DismantleBarUI");
	dismantleBar->SetOffset(glm::vec2(dismantleBar->GetFrameWidth() / 2, 1.0f));
	dismantleEmptyBar = new GLImage("UI/DismantleEmptyBarUI");

	handItem = new ItemInfo();

	return S_OK;
}

void HandUI::Release()
{
	SAFE_DELETE(allItemImage);
	SAFE_DELETE(background);
	SAFE_DELETE(backgroundName);
	SAFE_DELETE(dismantleBar);
	SAFE_DELETE(dismantleEmptyBar);

	SAFE_DELETE(handItem);
}

void HandUI::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Q'))
	{
		// 건축물 빠른 들기 기능
		if (handItem->IsEmpty())
		{
			Tile* tile = TileManager::GetSingleton()->GetLPTileUnderMouse();
			if (tile)
			{
				Structure* structure = tile->GetLpSturcture();
				if (structure)
				{
					EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->FindItem(structure->GetItemId())->MoveAllItemTo(handItem);
				}
			}
		}
		// 손 비우기 기능
		else
		{
			EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(ItemInfo(*handItem));
			handItem->amount = 0;
		}
	}
	
	// UI 위에 마우스가 있을 때 적용
	if (UIManager::GetSingleton()->IsMouseOnUI())
	{
		
	}
	// UI 위에 마우스가 없을 때 적용
	else 
	{
		// 아이템 버리기 기능
		if (KeyManager::GetSingleton()->IsOnceKeyDown('Z'))
		{
			if (!handItem->IsEmpty())
			{
				handItem->IncreaseAmount(-1);

				EntityManager::GetSingleton()->AddItemOnGround(handItem->id, g_cursorPosition.x, g_cursorPosition.y);
			}
		}

		dismantleBar->SetScale(glm::vec2(dismantlePercent / 0.6f, 1.0f));
	}

}

void HandUI::Render(ShaderProgram* shader)
{
	bool isMouseOnUI = UIManager::GetSingleton()->IsMouseOnUI();

	if (!handItem->IsEmpty())
	{
		if (!g_itemSpecs[handItem->id].buildable || isMouseOnUI)
		{
			glm::ivec2 maxFrame = allItemImage->GetMaxFrame();
			allItemImage->Render(shader, static_cast<float>(g_ptMouse.x + 16), static_cast<float>(g_ptMouse.y - 16), handItem->id % maxFrame.x, maxFrame.y - 1 - handItem->id / maxFrame.y);
		}
		// 핸드의 아이템 개수 출력
		TextRenderer::GetSingleton()->RenderText(to_string(handItem->amount), static_cast<float>(g_ptMouse.x + 16 - to_string(handItem->amount).length() * 6 + 17), g_ptMouse.y - 16 - 7.0f, 0.46f);
	}

	// UI 위에 마우스가 있을 때 적용
	if (isMouseOnUI)
	{

	}
	// UI 위에 마우스가 없을 때 적용
	else
	{
		Tile* tile = TileManager::GetSingleton()->GetLPTileUnderMouse();
		if (tile)
		{
			Structure* structure = tile->GetLpSturcture();
			if (structure)
			{
				background->Render(shader, g_currScreenSize.x - 128, g_currScreenSize.y / 2);
				backgroundName->Render(shader, g_currScreenSize.x - 128, g_currScreenSize.y / 2 - 7);
				structure->RenderInScreen(shader, g_currScreenSize.x - 128, g_currScreenSize.y / 2 + 128);
				TextRenderer::GetSingleton()->RenderText(" " + g_itemSpecs[structure->GetItemId()].name, g_currScreenSize.x - 250, g_currScreenSize.y / 2, 0.8f, glm::vec3(0.0f));
				TextRenderer::GetSingleton()->RenderText(structure->ToString(), g_currScreenSize.x - 250, g_currScreenSize.y / 2 - 40, 0.6f);

				if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RBUTTON))
				{
					if (structure != dismantleStructure)
					{
						dismantleStructure = structure;
						dismantlePercent = 0.0f;
					}
					else
					{
						if (dismantlePercent >= 0.6f)
						{
							EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(ItemInfo(dismantleStructure->GetItemId(), 1));
							EntityManager::GetSingleton()->DeleteEntity(dismantleStructure);
							glm::vec2 coordSize = dismantleStructure->GetCoordSize();
							// rotate sound
							if (coordSize.x + coordSize.y > 4)
								SoundManager::GetSingleton()->Play("Deconstruct-large", 0.6f);
							else if (coordSize.x + coordSize.y > 2)
								SoundManager::GetSingleton()->Play("Deconstruct-medium", 0.6f);
							else
								SoundManager::GetSingleton()->Play("Deconstruct-small", 0.6f);

							dismantlePercent = 0.0f;
						}
						dismantlePercent += TimerManager::GetSingleton()->GetTimeElapsed();
					}

					dismantleEmptyBar->Render(shader, g_currScreenSize.x / 2, 150);
					dismantleBar->Render(shader, g_currScreenSize.x / 2 - dismantleBar->GetFrameWidth() / 2, 150);
				}
			}
		}
	}

}

bool HandUI::IsEmpty()
{
	return handItem->amount == 0;
}
