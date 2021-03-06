﻿#pragma once
#include "GameNode.h"

class Entity : public GameNode
{
protected:
	Vec2 position;	// 엔티티의 위치
	bool passable;	// 캐릭터가 지나갈 수 있는지 여부

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* lpShader) {};
	virtual void Render(Shader* lpShader) = 0;
	virtual void LateRender(Shader* lpShader) {};

	bool IsPassable()			{ return passable; }
	Vec2 GetPosition()			{ return position; }
	virtual FRECT GetCollisionFRect()	{ return FRECT(); }
	virtual FRECT GetFRect()		{ return FRECT(); }

	virtual void SetPosition(Vec2 position) { this->position = position; }

	Entity()
		: passable(false)
	{};
	~Entity() {};
};
