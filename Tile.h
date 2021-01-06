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
	glm::ivec2 index;
	KIND kind;
	Ore* ore;
	Structure* structure;
public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();

	glm::ivec2 GetIndex() { return index; }
	KIND GetKind()	{ return kind; }
	Ore* GetLpOre() { return ore; }
	Structure* GetLpSturcture() { return structure; }
	void LinkStructure(Structure* structure) { this->structure = structure; }
	void UnlinkStructure() { this->structure = nullptr; }

	Tile()
		: ore(nullptr)
		, structure(nullptr)
	{};
};

