#include "Structure.h"
#include "StructureBuilder.h"
#include "Tile.h"
#include "HandUI.h"
#include "AssemblingMachine1.h"
#include "BurnerMiningDrill.h"
#include "TransportBelt.h"
#include "BurnerInserter.h"
#include "StoneFurnace.h"

HRESULT Structure::Init(int x, int y, DIRECTION direction, bool temp)
{
	this->position = { (float)x, (float)y };
	this->direction = direction;
	this->temp = temp;
	this->coord = ivec2(POS_TO_COORD(position));
	this->passable = g_itemSpecs[itemId].passable;
	this->coordSize = g_itemSpecs[itemId].coordSize;

	if (this->temp)
		return S_OK;

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
	if (this->temp)
		return;

	for (int y = 0; y < coordSize.y; y++)
	{
		for (int x = 0; x < coordSize.x; x++)
		{
			Tile* tile = TileManager::GetSingleton()->GetLpTile(
				coord.x - coordSize.x / 2 + x,
				coord.y - coordSize.y / 2 + y
			);
			tile->UnlinkStructure();
		}
	}
}

void Structure::Update()
{
	if (EntityManager::GetSingleton()->GetLpStructureBuilder()->IsActive())
	{
		if (temp && KeyManager::GetSingleton()->IsOnceKeyDown('R'))
		{
			Rotate();
		}
	}
	else if (PtInFRect(GetFRect(), g_cursorPosition))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown('R'))
		{
			Rotate();
		}

		if (usingClickEvent)
		{
			if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
			{
				SoundManager::GetSingleton()->Play("Machine-open", 0.4f);
				ClickEvent();
			}
		}
	}

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
	case BURNER_INSERTER:		return new BurnerInserter();
	case STONE_FURNACE:			return new StoneFurnace();
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

void Structure::Rotate()
{			
	if (g_itemSpecs[itemId].directionCount > 1)
	{
		// rotate sound
		if (coordSize.x + coordSize.y > 4)
			SoundManager::GetSingleton()->Play("Rotate-big", 0.6f);
		else if (coordSize.x + coordSize.y > 2)
			SoundManager::GetSingleton()->Play("Rotate-medium", 0.6f);
		else
			SoundManager::GetSingleton()->Play("Rotate-small", 0.6f);

		direction = (DIRECTION)RIGHT_DIR(direction);
	}
}
