#pragma once
#include "GameNode.h"

class Shader;
class GLImage;
class Tile : public GameNode
{
	int x;
	int y;
	GLImage* image;

public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};

