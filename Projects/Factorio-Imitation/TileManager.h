#pragma once
#include "framework.h"
#include "2DFramework/GameNode.h"

class Tile;
class Chunk;
class Shader;
class GLImage;
class VertexArrayObject;
class TileManager : public Singleton<TileManager>
{
private:
	map<int, map<int, Chunk*>> mapChunks;
	map<int, map<int, Chunk*>>::iterator bigIt;
	map<int, Chunk*>::iterator smallIt;

	vector<Chunk*> vecChunkInScreen;
	vector<Chunk*>::iterator vecChunkIt;

	GLImage* tileImages;

	glm::vec2 oreCurrFrame[1024];
	glm::vec2 oreOffset[1024];

	map<int, GLImage*> mapOreImages;

	Shader* instancingShader;
	VertexArrayObject* tilesVAO;
	glm::vec2* tileCurrFrame;
	glm::vec2* tileOffset;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(RECT cameraRect);

	Tile* GetLpTile(int coordX, int coordY);
	Tile* GetLpTile(float positionX, float positionY);
	Tile* GetLPTileUnderMouse();
	Chunk* GetLpChunk(int x, int y);

	TileManager() 
		: tileImages(nullptr)
	{};
};

