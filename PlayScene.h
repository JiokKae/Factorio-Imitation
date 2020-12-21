#pragma once
#include "Scene.h"

class Camera;
class Shader;
class Texture;
class GLImage;
class PlayScene : public Scene
{
	Camera* camera;
	Shader* lightingShader;

	GLImage* dirt_1Image;

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

