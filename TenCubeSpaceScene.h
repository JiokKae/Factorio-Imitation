#pragma once
#include "GameNode.h"

class Shader;
class Camera;
class TenCubeSpaceScene : public GameNode
{
private:
	int Width, Height;
	GLint EnvMode = GL_REPLACE;
	GLint TexFilter = GL_LINEAR;

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	unsigned int texture1;
	unsigned int texture2;
	float radio = 0.5;

	Shader* ourShader = nullptr;
	Camera* camera = nullptr;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	TenCubeSpaceScene();
	virtual ~TenCubeSpaceScene();
};

