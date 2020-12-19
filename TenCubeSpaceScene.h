#pragma once
#include "Scene.h"

class Shader;
class Camera;
class TenCubeSpaceScene : public Scene
{
private:
	int Width, Height;
	GLint EnvMode = GL_REPLACE;
	GLint TexFilter = GL_LINEAR;

	unsigned int VBO;
	unsigned int VAO;

	unsigned int texture1;
	unsigned int texture2;
	float radio = 0.5;

	Shader* ourShader;
	Camera* camera;

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

