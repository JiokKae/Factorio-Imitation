#include "ItemOnGround.h"
#include "Tile.h"
#include "Structure.h"
#include "TransportBelt.h"

void ItemOnGround::Init(ItemEnum _itemId, float _positionX, float _positionY)
{
	itemId = _itemId;
	position.x = _positionX;
	position.y = _positionY;
	parentTile = TileManager::GetSingleton()->GetLpTile(position.x, position.y);
	parentTile->LinkItemOnGround(this);
}

void ItemOnGround::Init(ItemEnum _itemId, glm::vec2 _position)
{
	Init(_itemId, _position.x, _position.y);
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

void ItemOnGround::SetPosition(Vec2 _position)
{
	this->position = _position;
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
