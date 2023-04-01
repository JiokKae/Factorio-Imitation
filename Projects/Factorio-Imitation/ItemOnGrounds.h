#pragma once
#include "../2DFramework/GameNode.h"
#include "framework.h"

class ItemOnGround;
class ItemOnGrounds : public GameNode
{
	vector<ItemOnGround*> vecItems;
	vector<ItemOnGround*>::iterator it;

	vector<ItemOnGround*> vecItemsInScreen;

	GLImage* allItemsImage;

	ShaderProgram* instancingShader;
	VertexArrayObject* itemsVAO;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update(FRECT cameraFRect);
	virtual void Render();

	void AddItem(ItemOnGround* item);
	void DeleteItem(ItemOnGround* item);

	ItemOnGrounds();
};

