#pragma once
#include "../2DFramework/GameNode.h"
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
	vector<Entity*> vecEntityInScreen;

	multimap<Vec2, Entity*>::iterator it;

	ItemOnGrounds* itemOnGrounds;

	Character* player;

	StructureBuilder* structureBuilder;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update(FRECT cameraFrect);
	virtual void Render(ShaderProgram* shader);

	Character* GetLpPlayer()			{ return player; }
	StructureBuilder* GetLpStructureBuilder()	{ return structureBuilder; }
	void Collision();

	void AddEntity(Entity* entity);
	void DeleteEntity(Entity* entity);

	void AddItemOnGround(int itemId, float positionX, float positionY);
	void DeleteItemOnGround(ItemOnGround* item);


	EntityManager()
		: player(nullptr)
		, itemOnGrounds(nullptr)
		, structureBuilder(nullptr)
	{};
	virtual ~EntityManager() {};
};

