﻿#pragma once
#include "Entity.h"
#include "ItemEnum.h"

class Structure : public Entity
{
public:
	// 구조물의 상태
	enum STATUS {
		NO_POWER,
		WORKING,
		WAITING_SPACE,
		WAITING_SOURCE_ITEMS,
		NO_MINABLE_RESOURCES,
		DESTORY,
		NO_RECIPE,
		ITEM_PRODUCTION_OVERLOAD,
		ITEM_INGREDIENT_SHORTAGE,
		END
	};
	static const char* statusString[STATUS::END];

protected:
	glm::ivec2 coord;
	glm::ivec2 coordSize;
	DIRECTION direction;
	ItemEnum itemId;
	STATUS status;		// 건물의 상태

	bool temp;		// 임시 건물인지
	bool usingClickEvent;	// 클릭 이벤트를 사용하는지

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false);
	virtual void Release() override;
	virtual void Update();
	virtual void Render(Shader* lpShader);
	virtual void Render(Shader* shader, float x, float y);

	static Structure* CreateStructure(ItemEnum itemId);

	void Rotate();
	virtual void ClickEvent() {};
	virtual bool InputItem(ItemInfo* item, glm::vec2 pos) { return false; };
	virtual bool OutputItem() { return false; };
	virtual bool TakeOutItem(ItemInfo* outItem) { return false; };
	virtual string ToString();

	FRECT GetFRect();
	ItemEnum GetItemId()			{ return itemId; }
	DIRECTION GetDirection()		{ return direction; }
	glm::ivec2 GetCoordSize()		{ return coordSize; }
	virtual FRECT GetCollisionFRect() override;

	Structure() {};
	~Structure() {};
};

