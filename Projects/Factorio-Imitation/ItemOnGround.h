#pragma once
#include "Entity.h"
#include "ItemId.h"

class ItemOnGround : public Entity
{
public:
	ItemOnGround(ItemId itemId, float positionX, float positionY);
	~ItemOnGround();

	void Update() override;
	
	virtual void SetPosition(const Vec2& position) override;
	virtual FRECT GetCollisionFRect() const override;
	ItemId GetItemId() const;
private:
	ItemId itemId;
	Tile* parentTile;
	bool positionChange;
};
