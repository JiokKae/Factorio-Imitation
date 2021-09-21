#pragma once
#include "Container.h"
class SteelChest :
    public Container
{
private:
	GLImage* mainImage;
	GLImage* shadowImage;
public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void FirstRender(Shader* shader) override;
	virtual void Render(Shader* shader) override;
	virtual void RenderInScreen(Shader* shader, float posX, float posY) override;

	virtual FRECT GetCollisionFRect() override;
};

