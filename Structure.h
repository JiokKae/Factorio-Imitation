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
	bool temp;
public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	static Structure* CreateStructure(ItemEnum itemId);

	int GetItemId() { return itemId; }
	DIRECTION GetDirection()	{ return direction; }
	glm::ivec2 GetCoordSize()	{ return coordSize; }
	FRECT GetFRect();
	virtual FRECT GetCollisionFRect() override;
	virtual bool InputItem(ItemInfo* item) { return false; };


	Structure() {};
	~Structure() {};
};

