#pragma once
#include "Singleton.h"
#include "framework.h"

class Recipe;
class RecipeManager : public Singleton<RecipeManager>
{
private:
	map<string, Recipe*> mapRecipes;

public:
	HRESULT Init();
	void Release();

	Recipe* AddRecipe(string strKey, ItemInfo output, float craftingTime, vector<ItemInfo> vecIngredients);
	void DeleteRecipe(string strKey);
	Recipe* FindRecipe(string strKey);
	Recipe* FindRecipe(int enumKey);
};

