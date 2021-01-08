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

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	static Structure* CreateStructure(ItemEnum itemId);

	int GetItemId() { return itemId; }
	DIRECTION GetDirection() { return direction; }
	FRECT GetFRect();
	virtual FRECT GetCollisionFRect() override;

	Structure() {};
	~Structure() {};
};

