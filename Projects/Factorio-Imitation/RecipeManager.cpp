#include "RecipeManager.h"
#include "Recipe.h"

HRESULT RecipeManager::Init()
{
	AddRecipe("Iron Plate",			ItemInfo(IRON_PLATE, 1),			3.2f,	{ ItemInfo(IRON_ORE, 1) });
	AddRecipe("Copper Plate",		ItemInfo(COPPER_PLATE, 1),			3.2f,	{ ItemInfo(COPPER_ORE, 1) });
	AddRecipe("Stone Brick",		ItemInfo(STONE_BRICK, 1),			3.2f,	{ ItemInfo(STONE, 2) });
	AddRecipe("Steel Plate",		ItemInfo(STEEL_PLATE, 1),			16.0f,	{ ItemInfo(IRON_PLATE, 5) });

	AddRecipe("Iron Gear Wheel",		ItemInfo(IRON_GEAR_WHEEL, 1),			0.5f,	{ ItemInfo(IRON_PLATE, 2) });
	AddRecipe("Copper Cable",		ItemInfo(COPPER_CABLE, 2),			0.5f,	{ ItemInfo(COPPER_PLATE, 1) });
	AddRecipe("Electronic Circuit",		ItemInfo(ELECTRONIC_CIRCUIT, 1),		0.5f,	{ ItemInfo(COPPER_CABLE, 3),		ItemInfo(IRON_PLATE, 1) });
	AddRecipe("Transport Belt",		ItemInfo(TRANSPORT_BELT, 2),			0.5f,	{ ItemInfo(IRON_GEAR_WHEEL, 1),		ItemInfo(IRON_PLATE, 1) });
	AddRecipe("Fast Transport Belt",	ItemInfo(FAST_TRANSPORT_BELT, 1),		0.5f,	{ ItemInfo(IRON_GEAR_WHEEL, 5),		ItemInfo(TRANSPORT_BELT, 1) });
	AddRecipe("Express Transport Belt",	ItemInfo(EXPRESS_TRANSPORT_BELT, 1),		0.5f,	{ ItemInfo(FAST_TRANSPORT_BELT, 1),	ItemInfo(IRON_GEAR_WHEEL, 10) });
	AddRecipe("Burner Inserter",		ItemInfo(BURNER_INSERTER, 1),			0.5f,	{ ItemInfo(IRON_GEAR_WHEEL, 1),		ItemInfo(IRON_PLATE, 1) });
	AddRecipe("Assembling Machine 1",	ItemInfo(ASSEMBLING_MACHINE_1, 1),		0.5f,	{ ItemInfo(ELECTRONIC_CIRCUIT, 3),	ItemInfo(IRON_GEAR_WHEEL, 5),		ItemInfo(IRON_PLATE, 9) });
	AddRecipe("Stone Furnace",		ItemInfo(STONE_FURNACE, 1),			0.5f,	{ ItemInfo(STONE, 5) });
	AddRecipe("Iron Chest",			ItemInfo(IRON_CHEST, 1),			0.5f,	{ ItemInfo(IRON_PLATE, 8) });
	AddRecipe("Steel Chest",		ItemInfo(STEEL_CHEST, 1),			0.5f,	{ ItemInfo(STEEL_PLATE, 8) });

	AddRecipePreset(STONE_FURNACE, { 
		IRON_PLATE, 
		COPPER_PLATE, 
		STONE_BRICK, 
		STEEL_PLATE 
	});
	AddRecipePreset(ASSEMBLING_MACHINE_1, { 
		IRON_GEAR_WHEEL, 
		COPPER_CABLE, 
		ELECTRONIC_CIRCUIT, 
		TRANSPORT_BELT, 
		FAST_TRANSPORT_BELT, 
		EXPRESS_TRANSPORT_BELT, 
		BURNER_INSERTER,
		ASSEMBLING_MACHINE_1,
		STONE_FURNACE,
		IRON_CHEST,
		STEEL_CHEST,
	});

	return S_OK;
}

void RecipeManager::Release()
{
	auto iter = recipes.begin();
	while (iter != recipes.end())
	{
		SAFE_DELETE(iter->second);
		iter = recipes.erase(iter);
	}
	recipes.clear();
	recipePresets.clear();

	ReleaseSingleton();
}

std::vector<Recipe*>* RecipeManager::FindRecipePreset(int itemId)
{
	if (recipePresets.contains(itemId) == true)
	{
		return &recipePresets[itemId];
	}

	return nullptr;
}

Recipe* RecipeManager::AddRecipe(const std::string& key, const ItemInfo& output, float craftingTime, const std::vector<ItemInfo>& ingredients)
{
	if (recipes.contains(key) == false)
	{
		recipes.emplace(key, new Recipe(ingredients, craftingTime, output));
	}

	return recipes[key];
}

Recipe* RecipeManager::FindRecipe(const std::string& key)
{
	if (recipes.contains(key) == true)
	{
		return recipes[key];
	}

	return nullptr;
}

Recipe* RecipeManager::FindRecipe(int itemId)
{
	return FindRecipe(g_itemSpecs[itemId].name);
}

void RecipeManager::AddRecipePreset(int itemId, const std::vector<int>& recipeKeys)
{
	std::vector<Recipe*> recipes;
	for (auto& recipeKey : recipeKeys)
	{
		if (Recipe* recipe = FindRecipe(recipeKey))
		{
			recipes.push_back(recipe);
		}
	}

	recipePresets.emplace(itemId, recipes);
}
