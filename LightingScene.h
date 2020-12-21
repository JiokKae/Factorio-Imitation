#pragma once
#include "Scene.h"
#include "framework.h"

class Camera;
class Shader;
class Texture;
class LightingScene : public Scene
{
	Camera* camera;
	Shader* lightingShader;
	Shader* lightCubeShader;

	unsigned int VBO, cubeVAO;
	unsigned int lightCubeVAO;

	Texture* diffuseMap;
	Texture* specularMap;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	LightingScene(int width, int height)
		: Scene(width, height)
		, camera(nullptr)
		, lightingShader(nullptr)
		, lightCubeShader(nullptr)
		, diffuseMap(nullptr)
		, specularMap(nullptr)
	{};
	virtual ~LightingScene();
};

