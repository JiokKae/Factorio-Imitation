#pragma once
#include "GameNode.h"
#include "framework.h"
#include <functional>
class Shader;
class Entity;
class Character;
class EntityManager : public GameNode
{
	multimap<Vec2, Entity*, greater<Vec2>> mapEntitys;
	multimap<Vec2, Entity*>::iterator it;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* shader, GLfloat playerPosY);
	void LateRender(Shader* shader, GLfloat playerPosY);

	void Collision(Character* player);

	void AddEntity(Entity* entity);
	void DeleteEntity(Entity* entity);

	EntityManager() 
	{};
	virtual ~EntityManager() {};
};

