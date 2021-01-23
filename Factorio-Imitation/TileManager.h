#pragma once
#include "GameNode.h"

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
	GLImage* tileImages;
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

