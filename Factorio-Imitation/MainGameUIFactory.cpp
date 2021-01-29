#include "MainGameUIFactory.h"
#include "CharacterUI.h"
#include "BurnerMiningDrillUI.h"
#include "StoneFurnaceUI.h""
#include "SelectRecipeAssemblingUI.h"
UI* MainGameUIFactory::CreateUI(UIEnum uiId)
{
	switch (uiId)
	{
	case CHARACTER_UI:					return new CharacterUI();
	case BURNER_MINING_DRILL_UI:		return new BurnerMiningDrillUI();
	case STONE_FURNACE_UI:				return new StoneFurnaceUI();
	case SELECT_RECIPE_ASSEMBLING_UI:	return new SelectRecipeAssemblingUI();
	}
	return nullptr;
}
