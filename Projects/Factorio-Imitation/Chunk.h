#pragma once
#include "framework.h"

class Tile;
class Chunk
{
public:
	Chunk(int x, int y);
	~Chunk();

	const RECT& GetRect() const;
	Tile* GetLpTile(int x, int y);
	const glm::ivec2& GetCoord() const;
	
private:
	std::vector<Tile*> tiles;
	RECT rect;
	glm::ivec2 coord;
};
