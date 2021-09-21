#include "Recipe.h"

HRESULT Recipe::Init(ItemInfo output, float craftingTime, vector<ItemInfo> vecIngredients)
{
	this->output = output;
	this->craftingTime = craftingTime;
	this->vecIngredients = vecIngredients;
	return S_OK;
}

void Recipe::Release()
{
}

bool Recipe::IsIngredient(int itemEnum)
{
	for (int i = 0; i < vecIngredients.size(); i++)
	{
		if (vecIngredients[i].id == itemEnum)
			return true;
	}
	return false;
}
