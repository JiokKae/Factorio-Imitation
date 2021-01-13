#pragma once
#include "Structure.h"

class UI;
class Shader;
class GLImage;
class Tile;
class Recipe;
class StoneFurnace : public Structure
{
public:
	enum STATUS {
		NO_POWER,
		NO_RECIPE,
		WORKING,
		ITEM_PRODUCTION_OVERLOAD,
		DESTORY,
		END
	};

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
	
	virtual bool InputItem(ItemInfo* inputItem, glm::vec2 pos) override;
	virtual void ClickEvent() override;
	
	Recipe* FindRecipeByIngredient(int itemEnum);

	float GetMaxPower() { return maxPower; }
	float GetCurrPower() { return currPower; }
	float GetProductionPercent() { return craftedTime; }
	ItemInfo* GetFuel() { return fuel; }

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

