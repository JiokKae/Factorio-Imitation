#pragma once
#include "GameNode.h"

class Shader;
class Ore;
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

public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();

	glm::ivec2 GetIndex() { return index; }
	KIND GetKind()	{ return kind; }
	Ore* GetLpOre() { return ore; }
	Tile()
		: ore(nullptr)

	{};
};

