#include "ItemOnGround.h"
#include "Tile.h"
#include "Structure.h"
#include "TransportBelt.h"

ItemOnGround::ItemOnGround(ItemId itemId, float _positionX, float _positionY)
	: itemId{ itemId }
{
	position.x = _positionX;
	position.y = _positionY;
	parentTile = TileManager::GetSingleton()->GetLpTile(position.x, position.y);
	parentTile->LinkItemOnGround(this);
}

ItemOnGround::~ItemOnGround()
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

void ItemOnGround::SetPosition(const Vec2& _position)
{
	this->position = _position;
	positionChange = true;
}

ItemId ItemOnGround::GetItemId() const
{
	return itemId;
}

FRECT ItemOnGround::GetCollisionFRect()
{
	return FRECT {
		.left = position.x - 16.0f,
		.top = position.y + 16.0f,
		.right = position.x + 16.0f,
		.bottom = position.y - 16.0f,
	};
}
