#pragma once
#include "Entity.h"
#include "ItemEnum.h"

class Shader;
class Structure : public Entity
{
protected:
	glm::ivec2 coordSize;
	DIRECTION direction;
	int itemId;

public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	static Structure* CreateStructure(ItemEnum itemId);

	FRECT GetFRect();
	virtual FRECT GetCollisionFRect() override;

	Structure() {};
	~Structure() {};
};

