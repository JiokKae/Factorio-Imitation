#pragma once
#include "Entity.h"
#include "ItemEnum.h"

class ItemOnGround : public Entity
{
    ItemEnum itemId;
    Tile* parentTile;

    bool positionChange;
public:
	virtual void Init(ItemEnum itemId, float positionX, float positionY);
	virtual void Init(ItemEnum itemId, glm::vec2 position);
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(Shader* shader) override {};
	
	virtual void SetPosition(Vec2 position) override;
	ItemEnum GetItemEnum() { return itemId; }
	virtual FRECT GetCollisionFRect() override;
};

