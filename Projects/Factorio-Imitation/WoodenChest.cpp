#include "WoodenChest.h"

HRESULT WoodenChest::Init(int x, int y, DIRECTION _direction, bool _temp)
{
	itemId = ItemEnum::WOODEN_CHEST;
	inventorySize = 16;

	mainImage = new GLImage("Entity/WoodenChest");

	shadowImage = new GLImage("Entity/WoodenChest-shadow");
	shadowImage->SetAlpha(0.5f);

	Container::Init(x, y, _direction, _temp);
	status = WORKING;

	return S_OK;
}

void WoodenChest::Release()
{
	Container::Release();

	SAFE_DELETE(mainImage);
	SAFE_DELETE(shadowImage);
}

void WoodenChest::FirstRender(ShaderProgram* shader)
{
	shadowImage->Render(shader, position.x + 22, position.y - 18);
}

void WoodenChest::Render(ShaderProgram* shader)
{
	mainImage->Render(shader, position.x, position.y);
}

void WoodenChest::RenderInScreen(ShaderProgram* shader, float posX, float posY)
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
