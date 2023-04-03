#include "Tile.h"
#include "Ore.h"

Tile::Tile(int x, int y)
	: coord(x, y)
	, structure{ nullptr }
	, ore{ new Ore(x, y) }
	, kind{ KIND::DIRT_1 }
{
}

Tile::~Tile()
{
	SAFE_DELETE(ore);
}

const glm::ivec2& Tile::GetCoord() const
{
	return coord;
}

Tile::KIND Tile::GetKind() const
{
	return kind;
}

Ore* Tile::GetLpOre()
{
	return ore;
}

Structure* Tile::GetLpSturcture()
{
	return structure;
}

const std::vector<ItemOnGround*>& Tile::GetItems() const
{
	return items;
}

void Tile::LinkStructure(Structure* _structure)
{
	structure = _structure;
}

void Tile::UnlinkStructure()
{
	structure = nullptr;
}

Tile* Tile::GetAroundTile(DIRECTION direction)
{
	switch (direction)
	{
	case NORTH:	return TileManager::GetSingleton()->GetLpTile(coord.x, coord.y + 1);
	case EAST:	return TileManager::GetSingleton()->GetLpTile(coord.x + 1, coord.y);
	case SOUTH:	return TileManager::GetSingleton()->GetLpTile(coord.x, coord.y - 1);
	case WEST:	return TileManager::GetSingleton()->GetLpTile(coord.x - 1, coord.y);
	}
	return nullptr;
}

void Tile::LinkItemOnGround(ItemOnGround* item)
{
	items.push_back(item);
}

void Tile::UnlinkItemOnGround(ItemOnGround* item)
{
	std::erase(items, item);
}
