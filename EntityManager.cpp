#include "EntityManager.h"
#include "Entity.h"
#include "Character.h"
#include "ItemOnGrounds.h"

HRESULT EntityManager::Init()
{    
    // player
    player = new Character();
    player->Init();

    itemOnGrounds = new ItemOnGrounds();
    itemOnGrounds->Init();
       
    return S_OK;
}

void EntityManager::Release()
{
    SAFE_RELEASE(itemOnGrounds);
    SAFE_RELEASE(player);

    mapEntitys.clear();
}

void EntityManager::Update(FRECT cameraFrect)
{
	for (it = mapEntitys.begin(); it != mapEntitys.end(); it++)
	{
        it->second->Update();
	}
    itemOnGrounds->Update(cameraFrect);
    player->Update();

    Collision();
}

void EntityManager::Render(Shader* shader)
{
    for (it = mapEntitys.begin(); it != mapEntitys.end(); it++)
    {
        //그림자 렌더
        it->second->FirstRender(shader);
    }
    for (it = mapEntitys.begin(); it != mapEntitys.end(); it++)
    {
        if (it->second->IsPassable())
            it->second->Render(shader);
    }
    itemOnGrounds->Render();
	for (it = mapEntitys.begin(); it != mapEntitys.end(); it++)
	{
		if (it->second->GetPosition().y > player->GetPosition().y && !it->second->IsPassable())
            it->second->Render(shader);
	}

    player->Render(shader);
}

void EntityManager::LateRender(Shader* shader)
{
	for (it = mapEntitys.begin(); it != mapEntitys.end(); it++)
	{
		if (it->second->GetPosition().y <= player->GetPosition().y && !it->second->IsPassable())
            it->second->Render(shader);
        it->second->LateRender(shader);
	}
}

void EntityManager::Collision()
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

void EntityManager::AddItemOnGround(ItemOnGround* item)
{
    itemOnGrounds->AddItem(item);
}
