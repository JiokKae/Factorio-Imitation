#pragma once
#include "GameNode.h"

class Shader;
class Entity : public GameNode
{
protected:
	glm::ivec2 position;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	virtual FRECT GetCollisionFRect() { return FRECT(); };
};
