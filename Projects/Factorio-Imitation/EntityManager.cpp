#include "EntityManager.h"
#include "Entity.h"
#include "Character.h"
#include "ItemOnGrounds.h"
#include "StructureBuilder.h"
#include "HandUI.h"

EntityManager::EntityManager()
	: player(nullptr)
	, itemOnGrounds(nullptr)
	, structureBuilder(nullptr)
{

}

EntityManager::~EntityManager()
{

}

HRESULT EntityManager::Init()
{
	// player
	player = new Character();
	player->Init();

	// item on ground
	itemOnGrounds = new ItemOnGrounds();

	// structure builder
	structureBuilder = new StructureBuilder();
	structureBuilder->Init();

	return S_OK;
}

void EntityManager::Release()
{
	SAFE_RELEASE(structureBuilder);
	SAFE_DELETE(itemOnGrounds);
	SAFE_RELEASE(player);

	for (auto& [coord, entity] : orderedEntities)
	{
		SAFE_RELEASE(entity);
	}
	orderedEntities.clear();

	ReleaseSingleton();
}

void EntityManager::Update(FRECT cameraFrect)
{
	entitiesInScreen.clear();
	const FRECT wideCamRect = cameraFrect + FRECT{ -TILE_SIZE, TILE_SIZE, TILE_SIZE, -TILE_SIZE } * 2;
	for (auto& [coord, entity] : orderedEntities)
	{
		entity->Update();
		if (PtInFRect(wideCamRect, entity->GetPosition()))
		{
			entitiesInScreen.push_back(entity);
		}
	}
	itemOnGrounds->Update(cameraFrect);
	player->Update();

	PlayerCollision();

	if (UIManager::GetSingleton()->IsMouseOnUI() == false)
	{
		const ItemInfo* handItem = UIManager::GetSingleton()->GetLpHandUI()->GetHandItem();
		if (handItem->IsEmpty() == false && g_itemSpecs[handItem->id].buildable)
		{
			structureBuilder->Activate(handItem->id);
		}
		else 
		{
			structureBuilder->Deactivate();
		}
	}
	else
	{
		structureBuilder->Deactivate();
	}

	structureBuilder->Update(player->GetLpPosition());
}

void EntityManager::Render(ShaderProgram* shader)
{
	for (Entity* entity : entitiesInScreen)
	{
		//그림자 렌더
		entity->FirstRender(shader);
	}
	for (Entity* entity : entitiesInScreen)
	{
		if (entity->IsPassable())
		{
			entity->Render(shader);
		}
	}

	itemOnGrounds->Render();

	for (Entity* entity : entitiesInScreen)
	{
		if (entity->GetPosition().y > player->GetPosition().y && entity->IsPassable() == false)
		{
			entity->Render(shader);
		}
	}

	player->Render(shader);

	for (Entity* entity : entitiesInScreen)
	{
		if (entity->GetPosition().y <= player->GetPosition().y && entity->IsPassable() == false)
		{
			entity->Render(shader); 
		}	
		entity->LateRender(shader);
	}

	structureBuilder->Render(shader);
}

void EntityManager::PlayerCollision()
{
	for (const Entity* entity : entitiesInScreen)
	{
		if (entity->IsPassable())
		{
			continue;
		}

		const FRECT playerRect = player->GetCollisionFRect();
		const FRECT entityRect = entity->GetCollisionFRect();

		FRECT collisionRect;
		if (IntersectFRect(&collisionRect, &playerRect, &entityRect) == false)
		{
			continue;
		}
		
		const float collisionWidth = std::fabs(collisionRect.right - collisionRect.left);
		const float collisionHeight = std::fabs(collisionRect.top - collisionRect.bottom);

		if (collisionWidth < collisionHeight)
		{
			// 우측 충돌일때
			if (collisionRect.right == entityRect.right)
			{
				player->GetLpPosition()->x += collisionWidth;
			}
			else
			{
				player->GetLpPosition()->x -= collisionWidth;
			}
		}
		else
		{
			// 아래측 충돌일 때
			if (collisionRect.bottom == entityRect.bottom)
			{
				player->GetLpPosition()->y -= collisionHeight;
			}
			else
			{
				player->GetLpPosition()->y += collisionHeight;
			}
		}
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	orderedEntities.emplace(entity->GetPosition(), entity);
}

void EntityManager::DeleteEntity(Entity* entity)
{
	auto it = orderedEntities.begin();
	while (it != orderedEntities.end())
	{
		if (it->second == entity)
		{
			SAFE_RELEASE(entity);
			orderedEntities.erase(it);
			break;
		}
		++it;
	}
}

void EntityManager::AddItemOnGround(int itemId, float positionX, float positionY)
{
	itemOnGrounds->AddItem(itemId, positionX, positionY);
}

void EntityManager::DeleteItemOnGround(ItemOnGround* item)
{
	itemOnGrounds->DeleteItem(item);
}
