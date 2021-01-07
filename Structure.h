#pragma once
#include "Entity.h"
#include "ItemEnum.h"

class Shader;
class Structure : public Entity
{
protected:
	glm::ivec2 coordSize;
	DIRECTION direction;

public:
	virtual HRESULT Init(int ItemId, int x, int y);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	Structure() {};
	~Structure() {};
};

