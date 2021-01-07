#include "Structure.h"
#include "AssemblingMachine1.h"
#include "BurnerMiningDrill.h"
#include "TransportBelt.h"
#include "Tile.h"

HRESULT Structure::Init(int x, int y)
{
	position = { x, y };
	coord = POS_TO_COORD(position);
	coordSize = g_itemSpecs[itemId].coordSize;

	for (int y = 0; y < coordSize.y; y++)
	{
		for (int x = 0; x < coordSize.x; x++)
		{
			Tile* tile = TileManager::GetSingleton()->GetLpTile(
				coord.x - coordSize.x / 2 + x, 
				coord.y - coordSize.y / 2 + y
			);
			tile->LinkStructure(this);
		}
	}
	
	return S_OK;
}

void Structure::Release()
{
	glm::ivec2 coord = POS_TO_COORD(position);

	for (int y = 0; y < coordSize.y; y++)
	{
		for (int x = 0; x < coordSize.x; x++)
		{
			Tile* tile = TileManager::GetSingleton()->GetLpTile(
				coord.x - coordSize.x / 2 + x,
				coord.y - coordSize.y / 2 + y
			);
			tile->LinkStructure(this);
		}
	}

	TileManager::GetSingleton()->GetLpTile((int)position.x / 64, (int)position.y / 64)->UnlinkStructure();
}

void Structure::Update()
{
}

void Structure::Render(Shader* lpShader)
{
}

Structure* Structure::CreateStructure(ItemEnum itemId)
{
	switch (itemId)
	{
	case ASSEMBLING_MACHINE_1:	return new AssemblingMachine1();
	case BURNER_MINING_DRILL:	return new BurnerMiningDrill();
	case TRANSPORT_BELT:		return new TransportBelt();
	}
	return nullptr;
}

FRECT Structure::GetFRect()
{
	FRECT rect;
	rect.left =		position.x - (coordSize.x * TILE_SIZE / 2.0f);
	rect.right =	position.x + (coordSize.x * TILE_SIZE / 2.0f);
	rect.top =		position.y + (coordSize.y * TILE_SIZE / 2.0f);
	rect.bottom =	position.y - (coordSize.y * TILE_SIZE / 2.0f);
	return rect;
}

FRECT Structure::GetCollisionFRect()
{
	FRECT rect;
	rect.left =		position.x - (coordSize.x * TILE_SIZE / 2.0f - TILE_SIZE * 0.3f);
	rect.right =	position.x + (coordSize.x * TILE_SIZE / 2.0f - TILE_SIZE * 0.3f);
	rect.top =		position.y + (coordSize.y * TILE_SIZE / 2.0f - TILE_SIZE * 0.3f);
	rect.bottom =	position.y - (coordSize.y * TILE_SIZE / 2.0f - TILE_SIZE * 0.3f);
	return rect;
}