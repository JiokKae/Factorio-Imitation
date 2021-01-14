#pragma once
#include "Entity.h"
#include "ItemEnum.h"

class Shader;
class Structure : public Entity
{
protected:
	glm::ivec2 coord;
	glm::ivec2 coordSize;
	DIRECTION direction;
	int itemId;

	bool temp;				// 임시 건물인지
	bool usingClickEvent;	// 클릭 이벤트를 사용하는지
public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	static Structure* CreateStructure(ItemEnum itemId);

	void Rotate();
	virtual void ClickEvent() {};
	virtual bool InputItem(ItemInfo* item, glm::vec2 pos) { return false; };
	virtual bool OutputItem() { return false; };
	virtual string ToString() { return string("Structure") + to_string(coord.x) + string(", ") + to_string(coord.y); };

	int GetItemId()				{ return itemId; }
	FRECT GetFRect();
	DIRECTION GetDirection()	{ return direction; }
	glm::ivec2 GetCoordSize()	{ return coordSize; }
	virtual FRECT GetCollisionFRect() override;

	Structure() {};
	~Structure() {};
};

