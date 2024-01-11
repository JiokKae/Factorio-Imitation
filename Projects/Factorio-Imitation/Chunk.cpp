#include "Chunk.h"
#include "Tile.h"

Chunk::Chunk(int x, int y)
	: tiles(TILE_IN_CHUNK_ROW * TILE_IN_CHUNK_ROW)
	, rect{ .left = x * CHUNK_SIZE,
		.top = (y + 1) * CHUNK_SIZE,
		.right = (x + 1) * CHUNK_SIZE,
		.bottom = y * CHUNK_SIZE,
	}
	, coord{ x, y }
{
	for (int _y = 0; _y < TILE_IN_CHUNK_ROW; _y++)
	{
		for (int _x = 0; _x < TILE_IN_CHUNK_ROW; _x++)
		{
			tiles[_y * TILE_IN_CHUNK_ROW + _x] = new Tile(_x + coord.x * TILE_IN_CHUNK_ROW, _y + coord.y * TILE_IN_CHUNK_ROW);
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

Tile* Chunk::GetLpTile(int x, int y)
{
	return tiles[y * TILE_IN_CHUNK_ROW + x];
}

const RECT& Chunk::GetRect() const
{
	return rect;
}

const glm::ivec2& Chunk::GetCoord() const
{
	return coord;
}
