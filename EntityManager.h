#pragma once
#include "GameNode.h"
#include "framework.h"
#include <functional>

class Entity;
class Character;
class ItemOnGround;
class ItemOnGrounds;
class EntityManager : public Singleton<EntityManager>
{
	multimap<Vec2, Entity*, greater<Vec2>> mapEntitys;
	multimap<Vec2, Entity*>::iterator it;

	ItemOnGrounds* itemOnGrounds;

	Character* player;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update(FRECT cameraFrect);
	virtual void Render(Shader* shader);
	void LateRender(Shader* shader);

	Character* GetPlayer() { return player; }

	void Collision();

	void AddEntity(Entity* entity);
	void AddItemOnGround(ItemOnGround* item);
	void DeleteEntity(Entity* entity);

	EntityManager() 
		: player(nullptr)
	{};
	virtual ~EntityManager() {};
};

