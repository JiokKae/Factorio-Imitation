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

bool Recipe::IsEnoughIngredient(vector<ItemInfo>* ingredients)
{
	for (int i = 0; i < vecIngredients.size(); i++)
	{
		// 만약 레시피보다 재료가 적거나 같지 않으면
		if (vecIngredients[i].amount > (*ingredients)[i].amount || vecIngredients[i].id != (*ingredients)[i].id)
			return false;
	}
	return true;
}
