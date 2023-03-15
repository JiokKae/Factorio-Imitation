#include "Recipe.h"
#include <algorithm>

Recipe::Recipe(const std::vector<ItemInfo>& ingredients, float craftingTime, const ItemInfo& output)
	: ingredients(ingredients)
	, output(output)
	, craftingTime(craftingTime)
{
}

const std::vector<ItemInfo>& Recipe::GetIngredients() const
{
	return ingredients;
}

bool Recipe::IsIngredient(int itemEnum) const
{
	return std::find_if(ingredients.cbegin(), ingredients.cend(), [itemEnum](const auto& ingredient) { return ingredient.id == itemEnum; }) != ingredients.end();
}

const ItemInfo& Recipe::GetOutput() const
{
	return output;
}

float Recipe::GetCraftingTime() const
{
	return craftingTime;
}
