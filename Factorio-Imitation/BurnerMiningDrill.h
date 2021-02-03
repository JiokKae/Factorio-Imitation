#pragma once
#include "Structure.h"

class UI;
class Shader;
class GLImage;
class Tile;
class BurnerMiningDrill : public Structure
{
private:
	// ���
	GLImage* image;
	glm::vec2 imageOffset[DIRECTION_END];
	GLImage* shadow;
	glm::vec2 shadowOffset[DIRECTION_END];
	float time;				
	
	// �������� ����
	float miningSpeed;			// ä�� ���ǵ�
	float energyConsumption;	// ä�� �Ҹ��Ŀ�	kw
	glm::ivec2 miningArea;		// ä�� ����
	float currPower;			// ���� �Ŀ�
	float maxPower;				// ����ϴ� ������ �ִ� �Ŀ�
	float productionPercent;	// �����

	// ������
	ItemInfo* waitingItemInfo;

	// Ÿ�� ����
	Tile** miningAreaTiles;
	Tile* targetTile;

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* shader) override;
	virtual void Render(Shader* shader) override;
	virtual void Render(Shader* shader, float posX, float posY) override;

	// �������� �޴� �Լ�
	// �����ǿ� �����ϴ� ��Ḧ �޾ƿ´�.
	virtual bool InputItem(ItemInfo* inputItem, glm::vec2 pos) override;

	// �������� �����ϴ� �Լ� 
	// ����ǰ�� �����Ѵ�.
	virtual bool OutputItem() override;

	// �������� Ŭ������ �� ����Ǵ� �Լ�
	// ȭ�� ä�� �帱�� UI�� �� �� �ִ�.
	virtual void ClickEvent() override;

	virtual string ToString() override;

	// ������
	float GetMaxPower()				{ return maxPower; }
	float GetCurrPower()			{ return currPower; }
	float GetProductionPercent()	{ return productionPercent; }	
	ItemInfo* GetWaitingItemInfo()	{ return waitingItemInfo; }
	bool IsMiningAreaEmpty();

	BurnerMiningDrill()
		: image(nullptr)
		, shadow(nullptr)
		, time(0.0f)
		, currPower(0.0f)
		, productionPercent(0.0f)
		, waitingItemInfo(nullptr)
		, miningAreaTiles(nullptr)
		, targetTile(nullptr)
	{};
	~BurnerMiningDrill() {};
};

