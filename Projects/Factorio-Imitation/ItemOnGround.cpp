#include "ItemOnGround.h"
#include "Tile.h"
#include "Structure.h"
#include "TransportBelt.h"

void ItemOnGround::Init(ItemEnum itemId, float positionX, float positionY)
{
	this->itemId = itemId;
	position.x = positionX;
	position.y = positionY;
	parentTile = TileManager::GetSingleton()->GetLpTile(position.x, position.y);
	parentTile->LinkItemOnGround(this);
}

void ItemOnGround::Init(ItemEnum itemId, glm::vec2 position)
{
	Init(itemId, position.x, position.y);
}

void ItemOnGround::Release()
{
	parentTile->UnlinkItemOnGround(this);
}

void ItemOnGround::Update()
{
	Structure* structure = parentTile->GetLpSturcture();
	if (structure && TransportBelt::IsTransportBelt(structure->GetItemId()))
	{
		TransportBelt* belt = (TransportBelt*)structure;
		belt->FlowItem(this, true);
	}

	// 만약 포지션이 변경되었다면
	if (positionChange)
	{
		// 부모 타일 바뀌었는지 검사
		Tile* currTile = TileManager::GetSingleton()->GetLpTile(position.x, position.y);
		if (parentTile != currTile)
		{
			parentTile->UnlinkItemOnGround(this);
			parentTile = currTile;
			parentTile->LinkItemOnGround(this);
		}
		positionChange = false;
	}
}

void ItemOnGround::SetPosition(Vec2 position)
{
	this->position = position;
	positionChange = true;
}

FRECT ItemOnGround::GetCollisionFRect()
{
	FRECT frect;

	frect.left		= position.x - 16.0f;
	frect.right		= position.x + 16.0f;
	frect.bottom	= position.y - 16.0f;
	frect.top		= position.y + 16.0f;

	return frect;
}
