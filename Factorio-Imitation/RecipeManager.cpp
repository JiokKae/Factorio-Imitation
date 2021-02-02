#include "RecipeManager.h"
#include "Recipe.h"

HRESULT RecipeManager::Init()
{
	AddRecipe("Iron Plate",				ItemInfo(IRON_PLATE, 1),				3.2f,	{ ItemInfo(IRON_ORE, 1) });
	AddRecipe("Copper Plate",			ItemInfo(COPPER_PLATE, 1),				3.2f,	{ ItemInfo(COPPER_ORE, 1) });
	AddRecipe("Stone Brick",			ItemInfo(STONE_BRICK, 1),				3.2f,	{ ItemInfo(STONE, 2) });
	AddRecipe("Steel Plate",			ItemInfo(STEEL_PLATE, 1),				16.0f,	{ ItemInfo(IRON_PLATE, 5) });

	AddRecipe("Iron Gear Wheel",		ItemInfo(IRON_GEAR_WHEEL, 1),			0.5f,	{ ItemInfo(IRON_PLATE, 2) });
	AddRecipe("Copper Cable",			ItemInfo(COPPER_CABLE, 2),				0.5f,	{ ItemInfo(COPPER_PLATE, 1) });
	AddRecipe("Electronic Circuit",		ItemInfo(ELECTRONIC_CIRCUIT, 1),		0.5f,	{ ItemInfo(COPPER_CABLE,	3),		ItemInfo(IRON_PLATE, 1) });
	AddRecipe("Transport Belt",			ItemInfo(TRANSPORT_BELT, 2),			0.5f,	{ ItemInfo(IRON_GEAR_WHEEL, 1),		ItemInfo(IRON_PLATE, 1) });
	AddRecipe("Fast Transport Belt",	ItemInfo(FAST_TRANSPORT_BELT, 1),		0.5f,	{ ItemInfo(IRON_GEAR_WHEEL, 5),		ItemInfo(TRANSPORT_BELT, 1) });
	AddRecipe("Express Transport Belt", ItemInfo(EXPRESS_TRANSPORT_BELT, 1),	0.5f,	{ ItemInfo(FAST_TRANSPORT_BELT, 1), ItemInfo(IRON_GEAR_WHEEL, 10) });
	AddRecipe("Burner Inserter",		ItemInfo(BURNER_INSERTER, 1),			0.5f,	{ ItemInfo(IRON_GEAR_WHEEL, 1),		ItemInfo(IRON_PLATE, 1) });
	AddRecipe("Assembling Machine 1",	ItemInfo(ASSEMBLING_MACHINE_1, 1),		0.5f,	{ ItemInfo(ELECTRONIC_CIRCUIT, 3),	ItemInfo(IRON_GEAR_WHEEL, 5), ItemInfo(IRON_PLATE, 9) });
	AddRecipe("Stone Furnace",			ItemInfo(STONE_FURNACE, 1),				0.5f,	{ ItemInfo(STONE, 5) });

	presetRecipes[STONE_FURNACE].push_back(FindRecipe(IRON_PLATE));
	presetRecipes[STONE_FURNACE].push_back(FindRecipe(COPPER_PLATE));
	presetRecipes[STONE_FURNACE].push_back(FindRecipe(STONE_BRICK));
	presetRecipes[STONE_FURNACE].push_back(FindRecipe(STEEL_PLATE));

	presetRecipes[ASSEMBLING_MACHINE_1].push_back(FindRecipe(IRON_GEAR_WHEEL));
	presetRecipes[ASSEMBLING_MACHINE_1].push_back(FindRecipe(COPPER_CABLE));
	presetRecipes[ASSEMBLING_MACHINE_1].push_back(FindRecipe(ELECTRONIC_CIRCUIT));
	presetRecipes[ASSEMBLING_MACHINE_1].push_back(FindRecipe(TRANSPORT_BELT));
	presetRecipes[ASSEMBLING_MACHINE_1].push_back(FindRecipe(FAST_TRANSPORT_BELT));
	presetRecipes[ASSEMBLING_MACHINE_1].push_back(FindRecipe(EXPRESS_TRANSPORT_BELT));
	presetRecipes[ASSEMBLING_MACHINE_1].push_back(FindRecipe(BURNER_INSERTER));
	presetRecipes[ASSEMBLING_MACHINE_1].push_back(FindRecipe(ASSEMBLING_MACHINE_1));
	presetRecipes[ASSEMBLING_MACHINE_1].push_back(FindRecipe(STONE_FURNACE));

	return S_OK;
}

void RecipeManager::Release()
{
	map<string, Recipe*>::iterator it = mapRecipes.begin();
	while (it != mapRecipes.end())
	{
		SAFE_RELEASE(it->second);
		it = mapRecipes.erase(it);
	}
	mapRecipes.clear();

	ReleaseSingleton();
}

Recipe* RecipeManager::AddRecipe(string strKey, ItemInfo output, float craftingTime, vector<ItemInfo> vecIngredients)
{
	Recipe* recipe = nullptr;
	recipe = FindRecipe(strKey);
	if (recipe)
	{
		return recipe;
	}
	recipe = new Recipe();
	if (FAILED(recipe->Init(output, craftingTime, vecIngredients)))
	{
		recipe->Release();
		delete recipe;

		return nullptr;
	}

	mapRecipes.insert(make_pair(strKey, recipe));

	return recipe;
}

void RecipeManager::DeleteRecipe(string strKey)
{
	map<string, Recipe*>::iterator it = mapRecipes.find(strKey);
	if (it != mapRecipes.end())
	{
		SAFE_RELEASE(it->second);
		mapRecipes.erase(it);
	}
}

Recipe* RecipeManager::FindRecipe(string strKey)
{
	map<string, Recipe*>::iterator it = mapRecipes.find(strKey);
	if (it != mapRecipes.end())
	{
		return it->second;
	}

	return nullptr;
}

Recipe* RecipeManager::FindRecipe(int enumKey)
{
	return FindRecipe(string(g_itemSpecs[enumKey].name));
}

vector<Recipe*>* RecipeManager::FindRecipes(int itemIdKey)
{
	return &presetRecipes[itemIdKey];
}
