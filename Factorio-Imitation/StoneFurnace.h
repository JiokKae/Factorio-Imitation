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
	GLImage* mainImage;
	GLImage* shadowImage;
	GLImage* fireImage;

	STATUS status;				// �������� ����
	float craftingSpeed;		// ���� �ӵ�
	float energyConsumption;	// ������ �Ҹ�	kw

	float currPower;
	float maxPower;
	float craftedTime;			// ������ �ð�
	float productionPercent;	// ���� �ۼ�Ʈ
	ItemInfo* fuel;
	ItemInfo* resource;
	ItemInfo* result;
	ItemInfo* usingResource;
	ItemInfo* newResult;

	vector<Recipe*> vecRecipes;
	Recipe* currRecipe;

	float time;				// �ð�

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* lpShader) override;
	virtual void Render(Shader* lpShader) override;
	virtual void Render(Shader* shader, float posX, float posY) override;

	virtual bool InputItem(ItemInfo* inputItem, glm::vec2 pos) override;
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
		, status(NO_POWER)
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

