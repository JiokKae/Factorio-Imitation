#pragma once
#include "framework.h"
#include "../2DFramework/GameNode.h"

class Entity : public GameNode
{
public:
	Entity();
	virtual ~Entity();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;

	virtual void FirstRender(ShaderProgram* lpShaderProgram) {};
	virtual void Render(ShaderProgram* lpShaderProgram) {};
	virtual void LateRender(ShaderProgram* lpShaderProgram) {};

	virtual void SetPosition(const Vec2& newPosition);

	virtual FRECT GetCollisionFRect() const;
	virtual FRECT GetFRect() const;

	const Vec2& GetPosition() const;
	bool IsPassable() const;

protected:
	Vec2 position;
	bool passable;
};
