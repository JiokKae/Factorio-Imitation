#pragma once
#include "Structure.h"

class UI;
class Shader;
class GLImage;
class Tile;
class BurnerMiningDrill : public Structure
{
public:
	enum STATUS {
		NOPOWER,
		WORKING,
		WAITING_SPACE,
		NO_MINABLE_RESOURCES,
		DESTORY,
		END
	};

private:
	GLImage* image;
	GLImage* shadow;
	glm::vec2 imageAniOffset[DIRECTION_END];
	glm::vec2 shadowAniOffset[DIRECTION_END];

	STATUS status;			// 채광 드릴의 상태
	float miningSpeed;		// 채광 스피드
	float miningPower;		// 채광 소모파워	kw
	glm::ivec2 miningArea;	// 채광 영역

	float currPower;
	float maxPower;
	float productionPercent;
	ItemInfo* waitingItemInfo;

	Tile** miningAreaTiles;
	Tile* targetTile;
	float time;				// 시간
public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* lpShader) override;
	virtual void Render(Shader* lpShader) override;

	float GetMaxPower()				{ return maxPower; }
	float GetCurrPower()			{ return currPower; }
	float GetProductionPercent()	{ return productionPercent; }	
	bool IsMiningAreaEmpty();

	BurnerMiningDrill()
		: image(nullptr)
		, shadow(nullptr)
		, status(NOPOWER)
		, currPower(0.0f)
		, productionPercent(0.0f)
		, waitingItemInfo(nullptr)
		, miningAreaTiles(nullptr)
		, targetTile(nullptr)
		, time(0.0f)
	{};
	~BurnerMiningDrill() {};
};

