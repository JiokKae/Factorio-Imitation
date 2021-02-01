#pragma once
#include "GameNode.h"

class Entity : public GameNode
{
protected:
	Vec2 position;
	bool passable;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* lpShader) {};
	virtual void Render(Shader* lpShader);
	virtual void LateRender(Shader* lpShader) {};

	bool IsPassable()	{ return passable; }
	Vec2 GetPosition()	{ return position; }
	virtual FRECT GetCollisionFRect() { return FRECT(); };
	virtual FRECT GetFRect() { return FRECT(); }

	virtual void SetPosition(Vec2 position) { this->position = position; }

	Entity() {};
	~Entity() {};
};
