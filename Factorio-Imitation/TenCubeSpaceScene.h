#pragma once
#include "Scene.h"

class Shader;
class FreeCamera;
class Texture;
class TenCubeSpaceScene : public Scene
{
private:
	int Width, Height;
	GLint EnvMode = GL_REPLACE;
	GLint TexFilter = GL_LINEAR;

	unsigned int VBO;
	unsigned int VAO;

	Texture* texture1;
	Texture* texture2;
	float radio = 0.5;

	Shader* ourShader;
	FreeCamera* camera;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	TenCubeSpaceScene(int width, int height)
		: Scene(width, height)
		, ourShader(nullptr)
		, camera(nullptr)
	{};

	virtual ~TenCubeSpaceScene();
};

