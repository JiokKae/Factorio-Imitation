#pragma once
#include "Structure.h"

class Shader;
class GLImage;
class BurnerMiningDrill : public Structure
{
	GLImage* image;
	GLImage* shadow;
	DIRECTION direction;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};

