#pragma once
#include "Structure.h"
class WoodenChest : public Structure
{
private:
	GLImage* mainImage;
	GLImage* shadowImage;
public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* shader) override;
	virtual void Render(Shader* shader) override;
	virtual void Render(Shader* shader, float posX, float posY) override;

	virtual FRECT GetCollisionFRect() override;
};

