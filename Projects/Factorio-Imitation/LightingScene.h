#pragma once
#include "framework.h"
#include "2DFramework/Scene/Scene.h"

class FreeCamera;
class Shader;
class Texture;
class LightingScene : public Scene
{
	FreeCamera* camera;
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
		, VBO(0)
		, cubeVAO(0)
		, lightCubeVAO(0)
	{};
	virtual ~LightingScene();
};

