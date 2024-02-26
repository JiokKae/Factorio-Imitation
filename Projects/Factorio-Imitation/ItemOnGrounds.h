#pragma once
#include "framework.h"

class ItemOnGround;
class Texture;

class ItemOnGrounds
{
public:
	ItemOnGrounds();
	~ItemOnGrounds();

	virtual void Update(const FRECT& cameraFRect);
	virtual void Render();

	void AddItem(int itemId, float positionX, float positionY);
	void DeleteItem(ItemOnGround* item);

private:
	std::vector<ItemOnGround*> items;
	std::vector<ItemOnGround*> itemsInScreen;

	Texture* allItemsTexture;
	ShaderProgram* instancingShader;
	VertexArrayObject* itemsVAO;

	void PickUp();
};
