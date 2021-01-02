#pragma once
#include "GameNode.h"

class Tile;
class Chunk;
class Shader;
class GLImage;
class VertexArrayObject;
class TileManager : public GameNode
{
private:
	map<int, map<int, Chunk*>> mapChunks;
	map<int, map<int, Chunk*>>::iterator bigIt;
	map<int, Chunk*>::iterator smallIt;
	GLImage* tileImages;
	GLImage* oreImages;

	Shader* instancingShader;
	VertexArrayObject* tilesVAO;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(RECT cameraRect);

	TileManager() 
		: tileImages(nullptr)
		, oreImages(nullptr)
	{};
};

