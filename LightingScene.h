#pragma once
#include "Scene.h"
#include "framework.h"

class Camera;
class Shader;
class Texture;
class LightingScene : public Scene
{
	glm::vec3 lightPos;
	Camera* camera;
	Shader* lightingShader;
	Shader* lightCubeShader;

	unsigned int VBO, cubeVAO;
	unsigned int lightCubeVAO;

	Texture* diffuseMap;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	LightingScene(int width, int height)
		: Scene(width, height)
	{};
	virtual ~LightingScene();
};

