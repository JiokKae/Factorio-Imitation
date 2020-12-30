#pragma once
#include "GameNode.h"

#define TILE_SIZE	64

class Tile;
class Chunk;
class Shader;
class GLImage;
class TileRenderer : public GameNode
{
private:
	map<int, map<int, Chunk*>> mapChunks;
	map<int, map<int, Chunk*>>::iterator bigIt;
	map<int, Chunk*>::iterator smallIt;
	GLImage* tileImages;
	GLImage* oreImages;

	Shader* instancingShader;
	UINT offsetVBO;
	UINT currFrameVBO;
	UINT tileQuadVBO;
	UINT tilesVAO;

	UINT oreVAO;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	TileRenderer() 
		: tileImages(nullptr)
		, oreImages(nullptr)
	{};
};

