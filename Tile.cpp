#include "Tile.h"
#include "GLImage.h"
#include "Shader.h"
#include "Ore.h"

HRESULT Tile::Init(int x, int y)
{
	coord.x = x;
	coord.y = y;

	kind = KIND::DIRT_1;

	ore = new Ore();
	ore->Init(x, y);

	return S_OK;
}

void Tile::Release()
{
	if (ore)
		ore->Release();
}

void Tile::Update()
{
	if (ore)
		ore->Update();
}

Tile* Tile::GetAroundTile(DIRECTION direction)
{
	switch (direction)
	{
	case NORTH: return TileManager::GetSingleton()->GetLpTile(coord.x, coord.y + 1);
	case EAST:	return TileManager::GetSingleton()->GetLpTile(coord.x + 1, coord.y);
	case SOUTH:	return TileManager::GetSingleton()->GetLpTile(coord.x, coord.y - 1);
	case WEST:	return TileManager::GetSingleton()->GetLpTile(coord.x - 1, coord.y);
	}
	return nullptr;
}
