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
	//image->Init("Checker", 320, 640, 4, 8);
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
	image->Render(lpShader, image->GetFrameWidth() * 2 * x, image->GetFrameHeight() * 2 * y, x % 64, y % 4);
}
