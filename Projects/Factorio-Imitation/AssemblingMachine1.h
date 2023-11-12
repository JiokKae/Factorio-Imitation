#pragma once
#include "Structure.h"

class UI;
class Recipe;
class AssemblingMachine1 : public Structure
{
public:
	// 조립 기계 내부의 재료들을 관리하는 클래스
	class Ingredients 
	{
		vector<ItemInfo> vecIngredients;
		vector<ItemInfo> vecUsingIngredients;

	public:
		HRESULT Init(Recipe* recipe);
		void Release();

		// 재료가 제작하기에 충분한지 반환하는 함수
		bool IsEnough(Recipe* recipe);
		
		// 재료를 소모하는 함수
		bool Consume(Recipe* recipe);

		ItemInfo& operator[](size_t index);
	};

private:
	// 출력
	GLImage* mainImage;
	GLImage* shadowImage;
	glm::vec2 shadowImageOffset;

	GLImage* altModeIcon;
	float time;

	// 구조물의 정보
	float craftingSpeed;		// 제작 속도
	size_t productsFinished;	// 생산 총량
	float craftedTime;		// 제작한 시간
	float productionPercent;	// 제작률

	// 아이템
	Ingredients ingredients;	// 재료들
	ItemInfo result;		// 생산품

	// 레시피
	vector<Recipe*>* vecRecipes;	// 레시피 목록
	Recipe* currRecipe;		// 현재 레시피
	size_t currRecipeIndex;		// 현재 레시피 번호

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void FirstRender(ShaderProgram* shader) override;
	virtual void Render(ShaderProgram* shader) override;
	virtual void RenderInScreen(ShaderProgram* shader, float posX, float posY) override;
	virtual void LateRender(ShaderProgram* shader) override;

	// 아이템을 받는 함수
	// 레시피에 맞는 재료를 받아온다.
	virtual bool InputItem(ItemInfo* inputItem, glm::vec2 position) override;
	
	// 아이템을 반출하는 함수 
	// 생산품을 반출한다.
	virtual bool TakeOutItem(ItemInfo* outItem) override;

	// 구조물을 클릭했을 때 실행되는 함수
	// 레시피를 전환할 수 있다.
	virtual void ClickEvent() override;

	virtual string ToString() override;
};

