#include "Recipe.h"

HRESULT Recipe::Init(ItemInfo _output, float _craftingTime, vector<ItemInfo> _vecIngredients)
{
	this->output = _output;
	this->craftingTime = _craftingTime;
	this->vecIngredients = _vecIngredients;
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
