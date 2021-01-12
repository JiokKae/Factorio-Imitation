#include "ItemOnGround.h"

void ItemOnGround::Init(ItemEnum itemId)
{
	this->itemId = itemId;
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
