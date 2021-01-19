#pragma once
#include "GameNode.h"
#include "framework.h"
#include <functional>

class Entity;
class Character;
class ItemOnGround;
class ItemOnGrounds;
class StructureBuilder;
class EntityManager : public Singleton<EntityManager>
{
	multimap<Vec2, Entity*, greater<Vec2>> mapEntitys;
	multimap<Vec2, Entity*>::iterator it;

	ItemOnGrounds* itemOnGrounds;

	Character* player;

	StructureBuilder* structureBuilder;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update(FRECT cameraFrect);
	virtual void Render(Shader* shader);

	Character* GetLpPlayer()					{ return player; }
	StructureBuilder* GetLpStructureBuilder()	{ return structureBuilder; }
	void Collision();

	void AddEntity(Entity* entity);
	void AddItemOnGround(ItemOnGround* item);
	void DeleteEntity(Entity* entity);

	EntityManager()
		: player(nullptr)
		, itemOnGrounds(nullptr)
		, structureBuilder(nullptr)
	{};
	virtual ~EntityManager() {};
};

