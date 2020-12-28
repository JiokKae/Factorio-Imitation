#pragma once
#include "GameNode.h"

class Tile;
class Chunk;
class Shader;
class GLImage;
class TileRenderer : public GameNode
{
private:
	map<int, map<int, Chunk*>> mapChunks;
	GLImage* tileImages;
	GLImage* oreImages;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	TileRenderer() 
		: tileImages(nullptr)
		, oreImages(nullptr)
	{};
};

