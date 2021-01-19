#pragma once
#include "TransportBelt.h"
class FastTransportBelt : public TransportBelt
{
    virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
    virtual void Release() override;
};

