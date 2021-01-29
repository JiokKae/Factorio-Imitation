#include "RecipeManager.h"
#include "Recipe.h"

HRESULT RecipeManager::Init()
{
	AddRecipe("Iron Plate",			ItemInfo(IRON_PLATE, 1),		3.2f,	{ ItemInfo(IRON_ORE, 1) });
	AddRecipe("Copper Plate",		ItemInfo(COPPER_PLATE, 1),		3.2f,	{ ItemInfo(COPPER_ORE, 1) });
	AddRecipe("Stone Brick",		ItemInfo(STONE_BRICK, 1),		3.2f,	{ ItemInfo(STONE, 2) });
	AddRecipe("Steel Plate",		ItemInfo(STEEL_PLATE, 1),		16.0f,	{ ItemInfo(IRON_PLATE, 5) });
	AddRecipe("Iron Gear Wheel",	ItemInfo(IRON_GEAR_WHEEL, 1),	0.5f,	{ ItemInfo(IRON_PLATE, 2) });

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
