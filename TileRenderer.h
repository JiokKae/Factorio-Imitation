#pragma once
#include "GameNode.h"

class Tile;
class Shader;
class GLImage;
class TileRenderer : public GameNode
{
private:
	Tile* tiles;
	GLImage* tileImages;
	GLImage* oreImages;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	TileRenderer() 
		: tiles(nullptr)
		, tileImages(nullptr)
		, oreImages(nullptr)
	{};
};

