#pragma once
#include "framework.h"

class Ore;
class Structure;
class Tile
{
public:
	enum class KIND {
		DIRT_1,
		END,
	};

	Tile(int x, int y);
	~Tile();

	const glm::ivec2& GetCoord() const;
	KIND GetKind() const;
	Ore* GetLpOre();
	Structure* GetLpSturcture();
	const std::vector<ItemOnGround*>& GetItems() const;
	void LinkStructure(Structure* _structure);
	void UnlinkStructure();
	Tile* GetAroundTile(DIRECTION direction);
	void LinkItemOnGround(ItemOnGround* item);
	void UnlinkItemOnGround(ItemOnGround* item);

private:
	std::vector<ItemOnGround*> items;
	glm::ivec2 coord;
	Structure* structure;
	Ore* ore;
	KIND kind;
};
