#pragma once
#include "Entity.h"
#include "ItemEnum.h"

class ItemOnGround : public Entity
{
public:
	ItemOnGround(ItemEnum itemId, float positionX, float positionY);
	~ItemOnGround();

	void Update() override;
	void Render(ShaderProgram*) override {};
	
	virtual void SetPosition(Vec2 position) override;
	virtual FRECT GetCollisionFRect() override;
	ItemEnum GetItemId() const;
private:
	ItemEnum itemId;
	Tile* parentTile;
	bool positionChange;
};
