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

	// spec
	float craftingSpeed;
	float craftedTime;
	float productionPercent;

	// slot
	Ingredients ingredients;
	ItemInfo result;

	Recipe* selectedRecipe;

	float time;

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* shader) override;
	virtual void Render(Shader* shader) override;
	virtual void Render(Shader* shader, float posX, float posY) override;

	// event
	virtual bool InputItem(ItemInfo* inputItem, glm::vec2 pos) override;
	virtual bool TakeOutItem(ItemInfo* outItem) override;
	virtual void ClickEvent() override;

	virtual string ToString() override;
};

