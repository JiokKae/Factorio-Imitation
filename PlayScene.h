#pragma once
#include "Scene.h"

class Camera;
class Shader;
class Texture;
class PlayScene : public Scene
{
	Camera* camera;
	Shader* lightingShader;
	unsigned int VBO;
	unsigned int VAO;
	Texture* diffuseMap;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	PlayScene(int width, int height)
		: Scene(width, height)
		, camera(nullptr)
	{};
	virtual ~PlayScene();
};

