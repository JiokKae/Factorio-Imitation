#pragma once
#include "framework.h"
#include "2DFramework/GameNode.h"

class Tile;
class Chunk;
class ShaderProgram;
class Structure;
class GLImage;
class VertexArrayObject;
class TileManager : public Singleton<TileManager>
{
private:
	std::map<int, std::map<int, Chunk*>> mapChunks;

	std::vector<Chunk*> vecChunkInScreen;

	std::map<int, GLImage*> tileImages;

	glm::vec2 oreCurrFrame[1024];
	glm::vec2 oreOffset[1024];

	map<int, GLImage*> mapOreImages;

	ShaderProgram* instancingShader;
	VertexArrayObject* tilesVAO;
	glm::vec2* tileCurrFrame;
	glm::vec2* tileOffset;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(const RECT& cameraRect);

	Tile* GetLpTile(int coordX, int coordY);
	Tile* GetLpTile(float positionX, float positionY);
	Tile* GetLpCursorTile();
	Structure* GetLpCursorStructure();
	Chunk* GetLpChunk(int x, int y);

	TileManager() {};
};

