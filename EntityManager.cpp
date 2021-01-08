#include "EntityManager.h"
#include "Entity.h"
#include "Character.h"

HRESULT EntityManager::Init()
{
    return S_OK;
}

void EntityManager::Release()
{
    mapEntitys.clear();
}

void EntityManager::Update()
{
	for (it = mapEntitys.begin(); it != mapEntitys.end(); it++)
	{
		it->second->Update();
	}
}

void EntityManager::Render(Shader* shader, GLfloat playerPosY)
{
	for (it = mapEntitys.begin(); it != mapEntitys.end(); it++)
	{
		if (it->second->GetPosition().y > playerPosY || it->second->IsPassable())
			it->second->Render(shader);
	}
}

void EntityManager::LateRender(Shader* shader, GLfloat playerPosY)
{
	for (it = mapEntitys.begin(); it != mapEntitys.end(); it++)
	{
		if (it->second->GetPosition().y <= playerPosY && !it->second->IsPassable())
			it->second->Render(shader);
        it->second->LateRender(shader);
	}
}

void EntityManager::Collision(Character* player)
{
    FRECT colRect;
    FRECT entityRect;
    FRECT playerRect = player->GetCollisionFRect();

    for (it = mapEntitys.begin(); it != mapEntitys.end(); it++)
    {
        Entity* entity = it->second;
        if (entity->IsPassable())
            continue;

        entityRect = entity->GetCollisionFRect();
        if (IntersectFRect(&colRect, &playerRect, &entityRect))
        {
            if (abs(colRect.right - colRect.left) < abs(colRect.top - colRect.bottom))
            {
                // 우측 충돌일때
                if (colRect.right == entityRect.right)
                    player->GetLpPosition()->x += abs(colRect.right - colRect.left);
                else
                    player->GetLpPosition()->x -= abs(colRect.right - colRect.left);
            }
            else
            {
                // 아래측 충돌일 때
                if (colRect.bottom == entityRect.bottom)
                    player->GetLpPosition()->y -= abs(colRect.top - colRect.bottom);
                else
                    player->GetLpPosition()->y += abs(colRect.top - colRect.bottom);
            }
        }
    }
}

void EntityManager::AddEntity(Entity* entity)
{
    mapEntitys.insert(make_pair(entity->GetPosition(), entity));
}

void EntityManager::DeleteEntity(Entity* entity)
{
	//vecEntitys.erase(std::remove(vecEntitys.begin(), vecEntitys.end(), entity), vecEntitys.end());

}
