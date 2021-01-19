#include "WoodenChest.h"

HRESULT WoodenChest::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::WOODEN_CHEST;
	Structure::Init(x, y, direction, temp);
	status = WORKING;

	mainImage = new GLImage();
	mainImage->Init("Entity/WoodenChest");

	shadowImage = new GLImage();
	shadowImage->Init("Entity/WoodenChest-shadow");
	shadowImage->SetAlpha(0.5f);

	return S_OK;
}

void WoodenChest::Release()
{
	Structure::Release();

	SAFE_RELEASE(mainImage);
	SAFE_RELEASE(shadowImage);
}

void WoodenChest::Update()
{
	Structure::Update();
}

void WoodenChest::FirstRender(Shader* shader)
{
	shadowImage->Render(shader, position.x + 22, position.y - 18);
}

void WoodenChest::Render(Shader* shader)
{
	Render(shader, position.x, position.y);
}

void WoodenChest::Render(Shader* shader, float posX, float posY)
{
	mainImage->Render(shader, posX, posY);
}

FRECT WoodenChest::GetCollisionFRect()
{
	FRECT rect;
	rect.left = position.x - (coordSize.x * TILE_SIZE / 2.0f);
	rect.right = position.x + (coordSize.x * TILE_SIZE / 2.0f);
	rect.top = position.y + (coordSize.y * TILE_SIZE / 2.0f);
	rect.bottom = position.y - (coordSize.y * TILE_SIZE / 2.0f);
	return rect;
}
