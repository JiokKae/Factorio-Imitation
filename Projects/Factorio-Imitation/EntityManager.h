#pragma once
#include "framework.h"
#include <functional>

class Entity;
class Character;
class ItemOnGround;
class ItemOnGrounds;
class StructureBuilder;
class EntityManager : public Singleton<EntityManager>
{
public:
	EntityManager();
	~EntityManager();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update(FRECT cameraFrect);
	virtual void Render(ShaderProgram* shader);

	Character* GetLpPlayer()			{ return player; }
	StructureBuilder* GetLpStructureBuilder()	{ return structureBuilder; }
	void PlayerCollision();

	void AddEntity(Entity* entity);
	void DeleteEntity(Entity* entity);

	void AddItemOnGround(int itemId, float positionX, float positionY);
	void DeleteItemOnGround(ItemOnGround* item);

private:
	std::multimap<Vec2, Entity*, std::greater<Vec2>> orderedEntities;
	std::vector<Entity*> entitiesInScreen;

	ItemOnGrounds* itemOnGrounds;

	Character* player;

	StructureBuilder* structureBuilder;
};
