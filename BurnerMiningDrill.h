#pragma once
#include "Structure.h"

class Shader;
class GLImage;
class BurnerMiningDrill : public Structure
{
	glm::ivec2 leftTopcoord;
	GLImage* image;
	GLImage* shadow;
	DIRECTION direction;
public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};

