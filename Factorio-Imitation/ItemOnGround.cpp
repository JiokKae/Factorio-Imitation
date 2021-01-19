#include "ItemOnGround.h"
#include "Tile.h"
#include "Structure.h"
#include "TransportBelt.h"

void ItemOnGround::Init(ItemEnum itemId)
{
	this->itemId = itemId;
}

void ItemOnGround::Update()
{
	glm::vec2 coord = POS_TO_COORD(position);
	Structure* structure = TileManager::GetSingleton()->GetLpTile(coord.x, coord.y)->GetLpSturcture();
	if (structure && TransportBelt::IsTransportBelt(structure->GetItemId()))
	{
		TransportBelt* belt = (TransportBelt*)structure;
		belt->FlowItem(this, true);
	}
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
