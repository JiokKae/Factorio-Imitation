#pragma once
#include "framework.h"
#include "ItemEnum.h"

class Recipe
{
	ItemInfo output;
	float craftingItme;
	vector<ItemInfo> vecIngredients;

public:
	HRESULT Init(ItemInfo output, float craftingTime, vector<ItemInfo> vecIngredients);
	void Release();

	bool IsIngredient(int itemEnum);

	float GetCraftingTime() { return craftingItme; }
};

