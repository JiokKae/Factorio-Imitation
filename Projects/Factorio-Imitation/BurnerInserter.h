#pragma once
#include "Structure.h"

class Tile;
class BurnerInserter : public Structure
{
	GLImage* platformImage;

	GLImage* handBaseImage;
	glm::vec2 handBaseScale;
	float handBaseAngle;
	float handBaseRenderAngle;

	GLImage* handOpenImage;
	glm::vec2 handOpenScale;
	float handOpenAngle;
	float handOpenRenderAngle;

	float handAngle;
	float handAngleSpeed;
	Tile* lpDirectionTile;
	Tile* lpDestTile;
	float destAngles[4] = { 0.0f, -90.0f, -180.0f, -270.0f };

	GLImage* allItemsImage;
	ItemInfo handItem;
public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* lpShader) override;
	virtual void Render(Shader* lpShader) override;
	virtual void LateRender(Shader* lpShader) override;
	virtual void RenderInScreen(Shader* shader, float posX, float posY) override;

	virtual string ToString() override;

	BurnerInserter();
};

