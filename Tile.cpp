#include "Tile.h"
#include "GLImage.h"
#include "Shader.h"
#include "Ore.h"

HRESULT Tile::Init(int x, int y)
{
	this->x = x;
	this->y = y;

	kind = KIND::DIRT_1;

	ore = new Ore();
	ore->Init();

	return S_OK;
}

void Tile::Release()
{
	if (ore)
		ore->Release();
}

void Tile::Update()
{
	if (ore)
		ore->Update();
}
