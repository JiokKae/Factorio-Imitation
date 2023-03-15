#pragma once
#include "framework.h"
#include "ItemEnum.h"

class Recipe
{
public:
	Recipe(const std::vector<ItemInfo>& ingredients, float craftingTime, const ItemInfo& output);

	const std::vector<ItemInfo>& GetIngredients() const;
	bool IsIngredient(int itemEnum) const;
	const ItemInfo& GetOutput() const;
	float GetCraftingTime() const;

private:
	std::vector<ItemInfo> ingredients;
	ItemInfo output;
	float craftingTime;
};
