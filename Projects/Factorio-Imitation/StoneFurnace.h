#pragma once
#include "Structure.h"

class UI;
class Shader;
class GLImage;
class Tile;
class Recipe;
class StoneFurnace : public Structure
{
private:
	// render
	GLImage* mainImage;
	GLImage* shadowImage;
	GLImage* fireImage;

	// spec
	float craftingSpeed;		// 제작 속도
	float energyConsumption;	// 에너지 소모량	kw
	float currPower;
	float maxPower;
	float craftedTime;			// 제작한 시간
	float productionPercent;	// 생산 퍼센트
	size_t productsFinished;	// 생산 총량

	// slot
	ItemInfo* fuel;
	ItemInfo* resource;
	ItemInfo* result;
	ItemInfo* usingResource;
	ItemInfo* newResult;

	vector<Recipe*>* vecRecipes;
	Recipe* currRecipe;

	float time;				// 시간

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* lpShader) override;
	virtual void Render(Shader* lpShader) override;
	virtual void RenderInScreen(Shader* shader, float posX, float posY) override;

	virtual bool InputItem(ItemInfo* inputItem, glm::vec2 pos) override;
	virtual bool TakeOutItem(ItemInfo* outItem) override;
	virtual void ClickEvent() override;
	
	Recipe* FindRecipeByIngredient(int itemEnum);
	virtual string ToString() override;


	float GetMaxPower()				{ return maxPower; }
	float GetCurrPower()			{ return currPower; }
	float GetProductionPercent()	{ return productionPercent; }
	ItemInfo* GetFuel()				{ return fuel; }
	ItemInfo* GetResource()			{ return resource; }
	ItemInfo* GetResult()			{ return result; }

	StoneFurnace()
		: mainImage(nullptr)
		, shadowImage(nullptr)
		, fireImage(nullptr)
		, currPower(0.0f)
		, craftedTime(0.0f)
		, fuel(nullptr)
		, resource(nullptr)
		, result(nullptr)
		, usingResource(nullptr)
		, newResult(nullptr)
		, currRecipe(nullptr)
		, time(0.0f)
	{};
	~StoneFurnace() {};
};

