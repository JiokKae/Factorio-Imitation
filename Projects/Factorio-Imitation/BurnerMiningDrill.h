#pragma once
#include "Structure.h"

class UI;
class Tile;
class BurnerMiningDrill : public Structure
{
private:
	// 출력
	std::vector<GLImage*> images;
	glm::vec2 imageOffset[DIRECTION_END];
	std::vector<GLImage*> shadows;
	glm::vec2 shadowOffset[DIRECTION_END];
	float time;				
	
	// 구조물의 정보
	float miningSpeed;		// 채광 스피드
	float energyConsumption;	// 채광 소모파워	kw
	glm::ivec2 miningArea;		// 채광 영역
	float currPower;		// 현재 파워
	float maxPower;			// 사용하는 연료의 최대 파워
	float productionPercent;	// 생산률

	// 아이템
	ItemInfo* waitingItemInfo;

	// 타일 정보
	Tile** miningAreaTiles;
	Tile* targetTile;

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* shader) override;
	virtual void Render(Shader* shader) override;
	virtual void RenderInScreen(Shader* shader, float posX, float posY) override;

	// 아이템을 받는 함수
	// 레시피에 존재하는 재료를 받아온다.
	virtual bool InputItem(ItemInfo* inputItem, glm::vec2 pos) override;

	// 아이템을 반출하는 함수 
	// 생산품을 반출한다.
	virtual bool OutputItem() override;

	// 구조물을 클릭했을 때 실행되는 함수
	// 화력 채광 드릴의 UI를 열 수 있다.
	virtual void ClickEvent() override;

	virtual string ToString() override;

	// 접근자
	float GetMaxPower()		{ return maxPower; }
	float GetCurrPower()		{ return currPower; }
	float GetProductionPercent()	{ return productionPercent; }	
	ItemInfo* GetWaitingItemInfo()	{ return waitingItemInfo; }
	bool IsMiningAreaEmpty();

	BurnerMiningDrill()
		: time()
		, currPower()
		, productionPercent()
		, energyConsumption()
		, maxPower()
		, miningSpeed()
		, waitingItemInfo()
		, miningAreaTiles(nullptr)
		, targetTile(nullptr)
	{};
	~BurnerMiningDrill() {};
};

