#pragma once
#include "GameNode.h"

#define CHUNK_IN_TILE	32

class Tile;
class Shader;
class Chunk : public GameNode
{
	glm::ivec2 index;
	Tile* tiles;

public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	glm::ivec2 GetIndex() { return index; }
	Tile* GetLpTile(int x, int y);

	Chunk() 
		: tiles(nullptr)
	{};
};

