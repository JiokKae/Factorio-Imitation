#pragma once
#include "framework.h"
#include "2DFramework/Scene/Scene.h"

class Shader;
class Camera;
class PointLight;
class TileManager;
class TextRenderer;
class DirectionalLight;
class DebugHelper;
class PlayScene : public Scene
{
	Camera* camera;
	Shader* lightingShader;
	GLuint uboMatrices;
	GLuint uboLights;
	GLuint uboUIMatrices;

	Shader* UIShader;
	TileManager* tileManager;

	TextRenderer* textRenderer;

	// light
	DirectionalLight* dirLight;
	PointLight* pointLights;
	int numOfPointLight;

	EntityManager* entityManager;

	DebugHelper* debugHelper;

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
		, tileManager(nullptr)
		, camera(nullptr)
		, textRenderer(nullptr)
		, entityManager(nullptr)
		, altMode(false)

	{};
	virtual ~PlayScene();
};

