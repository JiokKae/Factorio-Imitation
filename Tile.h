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
	int x;
	int y;
	KIND kind;
	Ore* ore;

public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();

	KIND GetKind()	{ return kind; }
	Ore* GetLpOre() { return ore; }
	Tile()
		: ore(nullptr)

	{};
};

