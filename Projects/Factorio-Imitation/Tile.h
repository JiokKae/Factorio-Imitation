#pragma once
#include "framework.h"
#include "2DFramework/GameNode.h"

class Ore;
class Structure;
class Tile : public GameNode
{
public:
	enum class KIND {
		DIRT_1,
		END,
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

	const glm::ivec2& GetCoord() const		{ return coord; }
	KIND GetKind() const				{ return kind; }
	Ore* GetLpOre()					{ return ore; }
	Structure* GetLpSturcture()			{ return structure; }
	const auto& GetItems() const			{ return items; }
	void LinkStructure(Structure* _structure)	{ structure = _structure; }
	void UnlinkStructure()				{ structure = nullptr; }
	Tile* GetAroundTile(DIRECTION direction);
	void LinkItemOnGround(ItemOnGround* item);
	void UnlinkItemOnGround(ItemOnGround* item);

	Tile()
		: ore(nullptr)
		, structure(nullptr)
	{};
};
