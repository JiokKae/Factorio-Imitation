#include "ExpressTransportBelt.h"
#include "Tile.h"

HRESULT ExpressTransportBelt::Init(int x, int y, DIRECTION _direction, bool _temp)
{
	itemId = ItemEnum::EXPRESS_TRANSPORT_BELT;
	speed = 135.0f;
	image = new GLImage();
	image->Init("Entity/ExpressTransportBelt", 32, 20);

	Structure::Init(x, y, _direction, _temp);
	status = WORKING;

	LinkAroundBelts();

	return S_OK;
}
