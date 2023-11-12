#pragma once
#include "Entity.h"
#include "ItemId.h"

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
	glm::ivec2 coord;	// 건축물의 기준 좌표
	glm::ivec2 coordSize;	// 건축물의 좌표상 크기
	DIRECTION direction;	// 건축물의 방향
	ItemId itemId;	// 건축물의 아이템 번호
	STATUS status;		// 건축물의 상태

	bool temp;		// 임시 건물인지
	bool usingClickEvent;	// 클릭 이벤트를 사용하는지

public:
	// Need to set ItemId
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false);
	virtual void Release() override;
	virtual void Update();
	virtual void Render(ShaderProgram* lpShader) = 0;
	virtual void RenderInScreen(ShaderProgram* shader, float x, float y) = 0;

	static Structure* CreateStructure(ItemId itemId);

	void Rotate();
	virtual void ClickEvent() {};

	// 모든 아이템이 투입되었을 때 True 반환
	virtual bool InputItem(ItemInfo* /*inputtedItem*/ , glm::vec2 /*position*/) { return false; };

	virtual bool OutputItem() { return false; };
	virtual bool TakeOutItem(ItemInfo* /*outItem*/) { return false; };
	virtual string ToString();

	FRECT GetFRect();
	ItemId GetItemId()			{ return itemId; }
	DIRECTION GetDirection()		{ return direction; }
	glm::ivec2 GetCoordSize()		{ return coordSize; }
	virtual FRECT GetCollisionFRect() override;

	Structure() {};
	~Structure() {};
};

