#pragma once
#include "framework.h"
#include "2DFramework/GameNode.h"

class Tile;
class Shader;
class Chunk : public GameNode
{
	glm::ivec2 coord;
	Tile* tiles;

public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();
	virtual void Render(ShaderProgram* lpShader);

	RECT GetRect();
	Tile* GetLpTile(int x, int y);
	glm::ivec2 GetCoord() { return coord; }

	Chunk() 
		: tiles(nullptr)
	{};
};

