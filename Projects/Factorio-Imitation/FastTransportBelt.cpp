#include "FastTransportBelt.h"
#include "Tile.h"

HRESULT FastTransportBelt::Init(int x, int y, DIRECTION _direction, bool _temp)
{
	itemId = ItemEnum::FAST_TRANSPORT_BELT;
	speed = 90.0f;
	image = new GLImage();
	image->Init("Entity/FastTransportBelt", 32, 20);

	Structure::Init(x, y, _direction, _temp);
	status = WORKING;

	LinkAroundBelts();

	return S_OK;
}
