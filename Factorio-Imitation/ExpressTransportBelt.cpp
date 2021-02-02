#include "ExpressTransportBelt.h"
#include "Tile.h"

HRESULT ExpressTransportBelt::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::EXPRESS_TRANSPORT_BELT;
	speed = 135.0f;
	Structure::Init(x, y, direction, temp);
	status = WORKING;

	image = new GLImage();
	image->Init("Entity/ExpressTransportBelt", 32, 20);

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
