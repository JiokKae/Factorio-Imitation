#pragma once
#include "framework.h"
#include <string>
#include <vector>

class Recipe;
class RecipeManager : public Singleton<RecipeManager>
{
public:
	HRESULT Init();
	void Release();

	std::vector<Recipe*>* FindRecipePreset(int itemId);
private:
	Recipe* AddRecipe(const std::string& key, const ItemInfo& output, float craftingTime, const std::vector<ItemInfo>& ingredients);
	Recipe* FindRecipe(const std::string& key);
	Recipe* FindRecipe(int itemId);

	void AddRecipePreset(int itemId, const std::vector<int>& recipeKeys);

	std::map<std::string, Recipe*> recipes;
	std::map<int, std::vector<Recipe*>> recipePresets;
};
