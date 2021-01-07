#pragma once
#include "GameNode.h"

class Shader;
class Entity : public GameNode
{
protected:
	glm::vec2 position;
	bool passable;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	bool IsPassable()		{ return passable; }
	glm::vec2 GetPosition() { return position; }

	virtual FRECT GetCollisionFRect() { return FRECT(); };

};
