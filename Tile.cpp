#include "Tile.h"
#include "GLImage.h"
#include "Shader.h"

HRESULT Tile::Init(int x, int y)
{
	this->x = x;
	this->y = y;

	// 텍스처 로드 및 이미지 생성
	image = new GLImage();
	image->Init("Terrain/Dirt_1", 2048, 288, 64, 9);
	image->SetZoom(2.0f);

	return S_OK;
}

void Tile::Release()
{
}

void Tile::Update()
{
}

void Tile::Render(Shader* lpShader)
{
	image->FrameRender(lpShader, 64 * x, 64 * y, 0, 0);
}
