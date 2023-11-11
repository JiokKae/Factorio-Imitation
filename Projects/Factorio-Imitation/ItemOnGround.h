#pragma once
#include "Entity.h"
#include "ItemId.h"

class ItemOnGround : public Entity
{
public:
	ItemOnGround(ItemId itemId, float positionX, float positionY);
	~ItemOnGround();

	void Update() override;
	void Render(ShaderProgram*) override {};
	
	virtual void SetPosition(Vec2 position) override;
	virtual FRECT GetCollisionFRect() override;
	ItemId GetItemId() const;
private:
	ItemId itemId;
	Tile* parentTile;
	bool positionChange;
};
