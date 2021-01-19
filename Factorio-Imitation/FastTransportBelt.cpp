#include "FastTransportBelt.h"
#include "Tile.h"

HRESULT FastTransportBelt::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::FAST_TRANSPORT_BELT;
	speed = 90.0f;
	Structure::Init(x, y, direction, temp);
	status = WORKING;

	image = new GLImage();
	image->Init("Entity/FastTransportBelt", 32, 20);

	aroundBelts = new TransportBelt * [DIRECTION_END]();
	Tile* tile = TileManager::GetSingleton()->GetLpTile(coord.x, coord.y);
	for (int i = 0; i < DIRECTION_END; i++)
	{
		Tile* aroundTile = tile->GetAroundTile((DIRECTION)i);
		if (aroundTile)
		{
			Structure* structure = aroundTile->GetLpSturcture();
			if (structure && IsTransportBelt(structure->GetItemId()))
			{
				aroundBelts[(DIRECTION)i] = (TransportBelt*)structure;
				if (!temp)
					((TransportBelt*)structure)->SetAroundBelts(DIRECTION((i + 2) % DIRECTION_END), this);
			}
		}
	}

	return S_OK;
}

void FastTransportBelt::Release()
{
	Structure::Release();

	SAFE_ARR_DELETE(aroundBelts);
	SAFE_DELETE(image);
}