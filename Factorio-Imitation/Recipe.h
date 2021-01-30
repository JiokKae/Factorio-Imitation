#pragma once
#include "framework.h"
#include "ItemEnum.h"

class Recipe
{
	ItemInfo output;
	float craftingTime;
	vector<ItemInfo> vecIngredients;

public:
	HRESULT Init(ItemInfo output, float craftingTime, vector<ItemInfo> vecIngredients);
	void Release();

	bool IsIngredient(int itemEnum);
	bool IsEnoughIngredient(vector<ItemInfo>* ingredients);

	ItemInfo GetOutput()				{ return output; }
	float GetCraftingTime()				{ return craftingTime; }
	ItemInfo GetIngredient(int index)	{ return vecIngredients[index]; }
	
	size_t size() const { return vecIngredients.size(); }
};

