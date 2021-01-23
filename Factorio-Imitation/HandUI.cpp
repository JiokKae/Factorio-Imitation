#include "HandUI.h"
#include "InventorySlotUI.h"
#include "ItemOnGround.h"
#include "Character.h"
#include "Inventory.h"
#include "Structure.h"
#include "Tile.h"

HRESULT HandUI::Init()
{
	allItemImage = new GLImage();
	allItemImage->Init("Icons/AllItems", 8, 8, 0.25f, 0.25f, 512, 512);

	background = new GLImage();
	background->Init("UI/BackgroundUI");
	background->SetAlpha(0.7f);

	backgroundName = new GLImage();
	backgroundName->Init("UI/BackgroundNameUI");

	dismantleBar = new GLImage();
	dismantleBar->Init("UI/DismantleBarUI");
	dismantleBar->SetOffset(glm::vec2(dismantleBar->GetFrameWidth() / 2, 1.0f));
	dismantleEmptyBar = new GLImage();
	dismantleEmptyBar->Init("UI/DismantleEmptyBarUI");

	handItem = new ItemInfo();

	return S_OK;
}

void HandUI::Release()
{
	SAFE_RELEASE(allItemImage);
	SAFE_RELEASE(background);
	SAFE_RELEASE(backgroundName);
	SAFE_RELEASE(dismantleBar);
	SAFE_RELEASE(dismantleEmptyBar);

	SAFE_DELETE(handItem);
}

void HandUI::Update()
{
	// 손 비우기 기능
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Q'))
	{
		if (handItem->amount != 0)
		{
			EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(new ItemInfo(*handItem));
			handItem->amount = 0;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown('Z'))
	{
		if (handItem->amount)
		{
			handItem->AddAmount(-1);

			ItemOnGround* item = new ItemOnGround();
			item->Init((ItemEnum)handItem->id, g_cursorPosition);
			EntityManager::GetSingleton()->AddItemOnGround(item);
		}
	}

	dismantleBar->SetScale(glm::vec2(dismantlePercent / 0.6f, 1.0f));
}

void HandUI::Render(Shader* shader)
{
	if (handItem->amount)
	{
		if (!g_itemSpecs[handItem->id].buildable || UIManager::GetSingleton()->IsMouseOnUI())
		{
			glm::ivec2 maxFrame = allItemImage->GetMaxFrame();
			allItemImage->Render(shader, g_ptMouse.x + 16, g_ptMouse.y - 16, handItem->id % maxFrame.x, maxFrame.y - 1 - handItem->id / maxFrame.y);
		}

		TextRenderer::GetSingleton()->RenderText(to_string(handItem->amount), g_ptMouse.x + 16 - to_string(handItem->amount).length() * 6 + 17, g_ptMouse.y - 16 - 7.0f, 0.46f);
	}
	if (!UIManager::GetSingleton()->IsMouseOnUI())
	{
		Tile* tile = TileManager::GetSingleton()->GetLPTileUnderMouse();
		if (tile)
		{
			Structure* structure = tile->GetLpSturcture();
			if (structure)
			{
				background->Render(shader, g_currScreenSize.x - 128, g_currScreenSize.y / 2);
				backgroundName->Render(shader, g_currScreenSize.x - 128, g_currScreenSize.y / 2 - 7);
				structure->Render(shader, g_currScreenSize.x - 128, g_currScreenSize.y / 2 + 128);
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
							EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(new ItemInfo(dismantleStructure->GetItemId(), 1));
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
