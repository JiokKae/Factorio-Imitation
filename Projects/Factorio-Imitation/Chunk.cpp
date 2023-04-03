#include "Chunk.h"
#include "Tile.h"

Chunk::Chunk(int x, int y)
	: rect{ .left = x * 2048,
		.top = (y + 1) * 2048,
		.right = (x + 1) * 2048,
		.bottom = y * 2048,
	}
	, coord(x, y)
{
	tiles.reserve(CHUNK_IN_TILE * CHUNK_IN_TILE);
	for (int _y = 0; _y < CHUNK_IN_TILE; _y++)
	{
		for (int _x = 0; _x < CHUNK_IN_TILE; _x++)
		{
			tiles.push_back(new Tile(_x + coord.x * CHUNK_IN_TILE, _y + coord.y * CHUNK_IN_TILE));
		}
	}
}

Chunk::~Chunk()
{
	for (Tile* tile : tiles)
	{
		SAFE_DELETE(tile);
	}
}

const RECT& Chunk::GetRect() const
{
	return rect;
}

Tile* Chunk::GetLpTile(int x, int y)
{
    return tiles[y * CHUNK_IN_TILE + x];
}

const glm::ivec2& Chunk::GetCoord() const
{
	return coord;
}
