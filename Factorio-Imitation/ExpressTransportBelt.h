#pragma once
#include "TransportBelt.h"
class ExpressTransportBelt : public TransportBelt
{
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
};

