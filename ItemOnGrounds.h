#pragma once
#include "GameNode.h"
#include "framework.h"

class ItemOnGround;
class ItemOnGrounds : public GameNode
{
	multimap<Vec2, ItemOnGround*, greater<Vec2>> mapItems;
	multimap<Vec2, ItemOnGround*>::iterator it;

	vector<ItemOnGround*> vecItemsInScreen;

	GLImage* allItemsImage;

	Shader* instancingShader;
	VertexArrayObject* itemsVAO;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update(FRECT cameraFRect);
	virtual void Render();

	void AddItem(ItemOnGround* item);
};

