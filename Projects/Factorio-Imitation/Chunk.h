#pragma once
#include "framework.h"

class Tile;
class Chunk
{
public:
	Chunk(int x, int y);
	~Chunk();

	Tile* GetLpTile(int x, int y);
	const RECT& GetRect() const;
	const glm::ivec2& GetCoord() const;
	
	static constexpr int CHUNK_SIZE{ TILE_IN_CHUNK_ROW * TILE_SIZE };
private:
	std::vector<Tile*> tiles;
	RECT rect;
	glm::ivec2 coord;
};
