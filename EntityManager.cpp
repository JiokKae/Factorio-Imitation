#include "EntityManager.h"
#include "Entity.h"

HRESULT EntityManager::Init()
{
    return S_OK;
}

void EntityManager::Release()
{
    vecEntitys.clear();
}

void EntityManager::Update()
{
	for (int i = 0; i < vecEntitys.size(); i++)
	{
		vecEntitys[i]->Update();
	}
}

void EntityManager::Render(Shader* shader)
{
	for (int i = 0; i < vecEntitys.size(); i++)
	{
		vecEntitys[i]->Render(shader);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
    vecEntitys.push_back(entity);
}

void EntityManager::DeleteEntity(Entity* entity)
{
	vecEntitys.erase(std::remove(vecEntitys.begin(), vecEntitys.end(), entity), vecEntitys.end());

}
