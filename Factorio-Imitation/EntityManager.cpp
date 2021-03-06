﻿#include "EntityManager.h"
#include "Entity.h"
#include "Character.h"
#include "ItemOnGrounds.h"
#include "StructureBuilder.h"
#include "HandUI.h"

HRESULT EntityManager::Init()
{
	// player
	player = new Character();
	player->Init();

	// item on ground
	itemOnGrounds = new ItemOnGrounds();
	itemOnGrounds->Init();

	// structure builder
	structureBuilder = new StructureBuilder();
	structureBuilder->Init();

	return S_OK;
}

void EntityManager::Release()
{
	SAFE_RELEASE(structureBuilder);
	SAFE_RELEASE(itemOnGrounds);
	SAFE_RELEASE(player);

	it = mapEntitys.begin();
	while (it != mapEntitys.end())
	{
		it = mapEntitys.erase(it);
	}
	mapEntitys.clear();

	ReleaseSingleton();
}

void EntityManager::Update(FRECT cameraFrect)
{
	vecEntityInScreen.clear();
	FRECT wideCamRect = cameraFrect + FRECT(-TILE_SIZE, TILE_SIZE, TILE_SIZE, -TILE_SIZE) * 2;
	for (it = mapEntitys.begin(); it != mapEntitys.end(); ++it)
	{
		it->second->Update();
		if (PtInFRect(wideCamRect, it->second->GetPosition()))
			vecEntityInScreen.push_back(it->second);
	}
	itemOnGrounds->Update(cameraFrect);
	player->Update();

	Collision();

	if (!UIManager::GetSingleton()->IsMouseOnUI())
	{
		ItemInfo* info = UIManager::GetSingleton()->GetLpHandUI()->GetHandItem();
		if (info->amount && g_itemSpecs[info->id].buildable)
			structureBuilder->Active(info->id);
		else
			structureBuilder->Deactive();
	}
	else
		structureBuilder->Deactive();

	structureBuilder->Update(player->GetLpPosition());
}

void EntityManager::Render(Shader* shader)
{
	vector<Entity*>::iterator it;
	for (it = vecEntityInScreen.begin(); it != vecEntityInScreen.end(); it++)
	{
		//그림자 렌더
		(*it)->FirstRender(shader);
	}
	for (it = vecEntityInScreen.begin(); it != vecEntityInScreen.end(); it++)
	{
		if ((*it)->IsPassable())
			(*it)->Render(shader);
	}
	itemOnGrounds->Render();
	for (it = vecEntityInScreen.begin(); it != vecEntityInScreen.end(); it++)
	{
		if ((*it)->GetPosition().y > player->GetPosition().y && !(*it)->IsPassable())
			(*it)->Render(shader);
	}

	player->Render(shader);

	for (it = vecEntityInScreen.begin(); it != vecEntityInScreen.end(); it++)
	{
		if ((*it)->GetPosition().y <= player->GetPosition().y && !(*it)->IsPassable())
			(*it)->Render(shader);
		(*it)->LateRender(shader);
	}

	structureBuilder->Render(shader);
}

void EntityManager::Collision()
{
	FRECT colRect;
	FRECT entityRect;
	FRECT playerRect = player->GetCollisionFRect();

	for (auto it = vecEntityInScreen.begin(); it != vecEntityInScreen.end(); it++)
	{
		Entity* entity = *it;
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
	it = mapEntitys.begin();
	while (it != mapEntitys.end())
	{
		if (it->second == entity)
		{
			SAFE_RELEASE(entity);
			mapEntitys.erase(it);
			break;
		}
		else
			++it;
	}

}

void EntityManager::AddItemOnGround(ItemOnGround* item)
{
	itemOnGrounds->AddItem(item);
}

void EntityManager::DeleteItemOnGround(ItemOnGround* item)
{
	itemOnGrounds->DeleteItem(item);
}
