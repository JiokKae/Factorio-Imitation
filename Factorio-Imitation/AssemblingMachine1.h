#pragma once
#include "Structure.h"

class UI;
class Shader;
class GLImage;
class Recipe;
class AssemblingMachine1 : public Structure
{
public:
	class Ingredients 
	{
		vector<ItemInfo> vecIngredients;
		vector<ItemInfo> vecUsingIngredients;

	public:
		HRESULT Init(Recipe* recipe);
		void Release();

		bool IsEnough(Recipe* recipe);
		
		bool Consume(Recipe* recipe);

		ItemInfo& operator[](int index);
	};

private:
	// render
	GLImage* image;
	GLImage* shadow;
	glm::vec2 imageAniOffset;
	glm::vec2 shadowAniOffset;
	GLImage* altModeIcon;

	// spec
	float craftingSpeed;
	float craftedTime;
	float productionPercent;

	size_t productsFinished;	// »ý»ê ÃÑ·®

	// slot
	Ingredients ingredients;
	ItemInfo result;

	vector<Recipe*>* vecRecipes;
	Recipe* currRecipe;
	size_t currRecipeIndex;

	float time;

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* shader) override;
	virtual void Render(Shader* shader) override;
	virtual void Render(Shader* shader, float posX, float posY) override;
	virtual void LateRender(Shader* shader) override;

	// event
	virtual bool InputItem(ItemInfo* inputItem, glm::vec2 pos) override;
	virtual bool TakeOutItem(ItemInfo* outItem) override;
	virtual void ClickEvent() override;

	virtual string ToString() override;
};

