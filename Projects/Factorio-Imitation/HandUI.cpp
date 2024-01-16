#include "HandUI.h"

#include "Character.h"
#include "Inventory.h"
#include "ItemOnGround.h"
#include "Structure.h"
#include "Tile.h"

HandUI::HandUI()
{
	allItemsImage = std::make_unique<GLImage>("Icons/AllItems", 8, 8, 0.25f, 0.25f);
	allItemsImage->SetScale(glm::vec2(0.25f));

	background = std::make_unique<GLImage>("UI/BackgroundUI");
	background->SetAlpha(0.7f);

	backgroundName = std::make_unique<GLImage>("UI/BackgroundNameUI");

	dismantleBar = std::make_unique<GLImage>("UI/DismantleBarUI");
	dismantleBar->SetOffset(glm::vec2(dismantleBar->GetFrameWidth() / 2, 1.0f));

	dismantleEmptyBar = std::make_unique<GLImage>("UI/DismantleEmptyBarUI");

	handItem = new ItemInfo();
}

HandUI::~HandUI()
{
	SAFE_DELETE(handItem);
}

void HandUI::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown('Q'))
	{
		if (handItem->IsEmpty() == true)
		{
			PipetteTool(TileManager::GetSingleton()->GetLpCursorStructure());
		}
		else
		{
			ClearCursor();
		}	
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown('Z'))
	{
		DropItem();
	}

	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RBUTTON))
	{
		DismantleStructure(TileManager::GetSingleton()->GetLpCursorStructure());
	}

	if (UIManager::GetSingleton()->IsMouseOnUI() == false)
	{
		dismantleBar->SetScale(glm::vec2(dismantleTime / 0.6f, 1.0f));
	}
}

void HandUI::Render(ShaderProgram* shader)
{
	if (handItem->IsEmpty() == false)
	{
		if (g_itemSpecs[handItem->id].buildable == false || UIManager::GetSingleton()->IsMouseOnUI() == true)
		{
			const glm::ivec2 maxFrame = allItemsImage->GetMaxFrame();
			allItemsImage->Render(shader, static_cast<float>(g_ptMouse.x + 16), static_cast<float>(g_ptMouse.y - 16), handItem->id % maxFrame.x, maxFrame.y - 1 - handItem->id / maxFrame.y);
		}

		// 핸드의 아이템 개수 출력
		const std::string handAmountString{std::to_string(handItem->amount)};
		TextRenderer::GetSingleton()->RenderText(handAmountString, static_cast<float>(g_ptMouse.x + 16 - handAmountString.length() * 6 + 17), g_ptMouse.y - 16 - 7.0f, 0.46f);
	}

	if (UIManager::GetSingleton()->IsMouseOnUI() == false)
	{
		if (Structure* cursorStructure = TileManager::GetSingleton()->GetLpCursorStructure())
		{
			RenderStructureInfoUI(shader, cursorStructure);

			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RBUTTON))
			{
				RenderDismantleProgressBar(shader);
			}
		}
	}
}

ItemInfo* HandUI::GetHandItem()
{
	return handItem;
}

void HandUI::PipetteTool(Structure* structure)
{
	if (structure == nullptr || UIManager::GetSingleton()->IsMouseOnUI() == true)
	{
		return;
	}

	EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->GetItem(structure->GetItemId(), handItem);
}

void HandUI::ClearCursor()
{
	EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(*handItem);
	handItem->amount = 0;
}

void HandUI::DropItem()
{
	if (handItem->IsEmpty() || UIManager::GetSingleton()->IsMouseOnUI() == true)
	{
		return;
	}

	handItem->IncreaseAmount(-1);

	EntityManager::GetSingleton()->AddItemOnGround(handItem->id, g_cursorPosition.x, g_cursorPosition.y);
}

void HandUI::DismantleStructure(Structure* structure)
{
	if (structure == nullptr || UIManager::GetSingleton()->IsMouseOnUI() == true)
	{
		return;
	}

	if (dismantleStructure != structure)
	{
		dismantleStructure = structure;
		dismantleTime = 0.0f;
		return;
	}

	if (dismantleTime >= 0.6f)
	{
		EntityManager::GetSingleton()->GetLpPlayer()->GetLpInventory()->AddItem(ItemInfo(dismantleStructure->GetItemId(), 1));

		dismantleStructure->PlayDeconstructSound();
		EntityManager::GetSingleton()->DeleteEntity(dismantleStructure);
		dismantleStructure = nullptr;

		dismantleTime = 0.0f;
	}
	dismantleTime += TimerManager::GetSingleton()->GetTimeElapsed();
}

void HandUI::RenderStructureInfoUI(ShaderProgram* shader, Structure* structure)
{
	background->Render(shader, g_currScreenSize.x - 128, g_currScreenSize.y / 2);
	backgroundName->Render(shader, g_currScreenSize.x - 128, g_currScreenSize.y / 2 - 7);

	structure->RenderInScreen(shader, g_currScreenSize.x - 128, g_currScreenSize.y / 2 + 128);

	TextRenderer::GetSingleton()->RenderText(" " + g_itemSpecs[structure->GetItemId()].name, g_currScreenSize.x - 250, g_currScreenSize.y / 2, 0.8f, glm::vec3(0.0f));
	TextRenderer::GetSingleton()->RenderText(structure->ToString(), g_currScreenSize.x - 250, g_currScreenSize.y / 2 - 40, 0.6f);
}

void HandUI::RenderDismantleProgressBar(ShaderProgram* shader)
{
	dismantleEmptyBar->Render(shader, g_currScreenSize.x / 2, DISMANTLE_POSITION_Y);
	dismantleBar->Render(shader, g_currScreenSize.x / 2 - dismantleBar->GetFrameWidth() / 2, DISMANTLE_POSITION_Y);
}
