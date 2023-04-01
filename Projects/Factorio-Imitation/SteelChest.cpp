#include "SteelChest.h"

HRESULT SteelChest::Init(int x, int y, DIRECTION _direction, bool _temp)
{
	itemId = ItemEnum::STEEL_CHEST;
	inventorySize = 48;

	mainImage = new GLImage("Entity/SteelChest");

	shadowImage = new GLImage("Entity/SteelChest-shadow");
	shadowImage->SetAlpha(0.5f);

	Container::Init(x, y, _direction, _temp);
	status = WORKING;

	return S_OK;
}

void SteelChest::Release()
{
	Container::Release();

	SAFE_DELETE(mainImage);
	SAFE_DELETE(shadowImage);
}

void SteelChest::FirstRender(ShaderProgram* shader)
{
	shadowImage->Render(shader, position.x + 22, position.y - 18);
}

void SteelChest::Render(ShaderProgram* shader)
{
	mainImage->Render(shader, position.x, position.y);
}

void SteelChest::RenderInScreen(ShaderProgram* shader, float posX, float posY)
{
	mainImage->Render(shader, posX, posY);
}

FRECT SteelChest::GetCollisionFRect()
{
	FRECT rect;
	rect.left = position.x - (coordSize.x * TILE_SIZE / 2.0f);
	rect.right = position.x + (coordSize.x * TILE_SIZE / 2.0f);
	rect.top = position.y + (coordSize.y * TILE_SIZE / 2.0f);
	rect.bottom = position.y - (coordSize.y * TILE_SIZE / 2.0f);

	return rect;
}
