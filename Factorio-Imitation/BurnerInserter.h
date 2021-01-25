#pragma once
#include "Structure.h"
class BurnerInserter : public Structure
{
	GLImage* platformImage;

	GLImage* handBaseImage;
	glm::vec2 handBaseScale;
	float handBaseAngle;

	GLImage* handOpenImage;
	glm::vec2 handOpenScale;
	float handOpenAngle;

	GLImage* allItemsImage;
	ItemInfo handItem;
public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* lpShader) override;
	virtual void Render(Shader* lpShader) override;
	virtual void LateRender(Shader* lpShader) override;
	virtual void Render(Shader* shader, float posX, float posY) override;

	virtual string ToString() override;
};

