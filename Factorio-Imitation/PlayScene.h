#pragma once
#include "Scene.h"

class Camera;
class PointLight;
class TileManager;
class TextRenderer;
class DirectionalLight;
class PlayScene : public Scene
{
	Camera* camera;
	Shader* lightingShader;
	GLuint uboMatrices;
	GLuint uboLights;
	GLuint uboUIMatrices;

	Shader* UIShader;
	TileManager* tileRenderer;

	TextRenderer* textRenderer;

	DirectionalLight* dirLight;
	PointLight* pointLights;
	int numOfPointLight;

	EntityManager* entityManager;

	bool altMode;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	bool IsAltMode() { return altMode; }

	PlayScene(int width, int height)
		: Scene(width, height)
		, lightingShader(nullptr)
		, tileRenderer(nullptr)
		, camera(nullptr)
		, textRenderer(nullptr)
		, entityManager(nullptr)
	{};
	virtual ~PlayScene();
};

