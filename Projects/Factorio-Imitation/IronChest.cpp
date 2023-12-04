#include "IronChest.h"

HRESULT IronChest::Init(int x, int y, DIRECTION _direction, bool _temp)
{
	itemId = ItemId::IRON_CHEST;
	inventorySize = 32;

	mainImage = new GLImage("Entity/IronChest");

	shadowImage = new GLImage("Entity/IronChest-shadow");
	shadowImage->SetAlpha(0.5f);

	Container::Init(x, y, _direction, _temp);
	status = WORKING;

	return S_OK;
}

void IronChest::Release()
{
	Container::Release();

	SAFE_DELETE(mainImage);
	SAFE_DELETE(shadowImage);
}

void IronChest::FirstRender(ShaderProgram* shader)
{
	shadowImage->Render(shader, position.x + 22, position.y - 18);
}

void IronChest::Render(ShaderProgram* shader)
{
	mainImage->Render(shader, position.x, position.y);
}

void IronChest::RenderInScreen(ShaderProgram* shader, float posX, float posY)
{
	mainImage->Render(shader, posX, posY);
}

FRECT IronChest::GetCollisionFRect() const
{
	FRECT rect;
	rect.left = position.x - (coordSize.x * TILE_SIZE / 2.0f);
	rect.right = position.x + (coordSize.x * TILE_SIZE / 2.0f);
	rect.top = position.y + (coordSize.y * TILE_SIZE / 2.0f);
	rect.bottom = position.y - (coordSize.y * TILE_SIZE / 2.0f);

	return rect;
}
