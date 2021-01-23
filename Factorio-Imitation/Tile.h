#pragma once
#include "GameNode.h"

class Shader;
class Ore;
class Structure;
class Tile : public GameNode
{
public:
	enum class KIND {
		DIRT_1,
		END
	};

private:
	glm::ivec2 coord;
	KIND kind;
	Ore* ore;
	Structure* structure;
	vector<ItemOnGround*> items;

public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();

	glm::ivec2 GetCoord() { return coord; }
	KIND GetKind()	{ return kind; }
	Ore* GetLpOre() { return ore; }
	Structure* GetLpSturcture() { return structure; }
	auto GetItems() { return items; }
	void LinkStructure(Structure* structure) { this->structure = structure; }
	void UnlinkStructure() { this->structure = nullptr; }
	Tile* GetAroundTile(DIRECTION direction);
	void LinkItemOnGround(ItemOnGround* item);
	void UnlinkItemOnGround(ItemOnGround* item);
	Tile()
		: ore(nullptr)
		, structure(nullptr)
	{};
};

