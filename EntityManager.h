#pragma once
#include "GameNode.h"
#include "framework.h"

class Shader;
class Entity;
class EntityManager : public GameNode
{
	vector<Entity*> vecEntitys;
	vector<Entity*>::iterator it;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* shader);

	int GetSize()					{ return vecEntitys.size(); }
	Entity* GetEntity(int index)	{ return vecEntitys[index]; }
	void AddEntity(Entity* entity);
	void DeleteEntity(Entity* entity);

	EntityManager() 
	{};
	virtual ~EntityManager() {};
};

