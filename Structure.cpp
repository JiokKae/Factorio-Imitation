#include "Structure.h"
#include "Tile.h"

HRESULT Structure::Init(int ItemId, int x, int y)
{
	position = { x, y };
	coordSize = g_itemSpecs[ItemId].coordSize;
	glm::ivec2 coord = POS_TO_COORD(position);

	for (int y = 0; y < coordSize.y; y++)
	{
		for (int x = 0; x < coordSize.x; x++)
		{
			Tile* tile = TileManager::GetSingleton()->GetLpTile(coord.x - coordSize.x / 2 * ((coord.x < 0) ? 0 : 1) + x, coord.y - coordSize.y / 2 * ((coord.y < 0) ? 0 : 1) + y);
			tile->LinkStructure(this);
		}
	}

	return S_OK;
}

void Structure::Release()
{
	TileManager::GetSingleton()->GetLpTile((int)position.x / 64, (int)position.y / 64)->UnlinkStructure();
}

void Structure::Update()
{
}

void Structure::Render(Shader* lpShader)
{
}
