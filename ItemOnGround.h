#pragma once
#include "Entity.h"
#include "ItemEnum.h"

class ItemOnGround : public Entity
{
    ItemEnum itemId;

public:
    virtual void Init(ItemEnum itemId);
    ItemEnum GetItemEnum() { return itemId; }
};

