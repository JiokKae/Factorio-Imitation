#pragma once
#include "Scene.h"

class Camera;
class Shader;
class Character;
class PointLight;
class CharacterUI;
class TileRenderer;
class TextRenderer;
class DirectionalLight;
class PlayScene : public Scene
{
	Camera* camera;
	Shader* lightingShader;
	unsigned int uboMatrices;
	unsigned int uboLights;

	Shader* UIShader;
	TileRenderer* tileRenderer;
	Character* player;
	CharacterUI* characterUI;
	TextRenderer* textRenderer;

	DirectionalLight* dirLight;
	PointLight* pointLights;
	int numOfPointLight;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	PlayScene(int width, int height)
		: Scene(width, height)
		, lightingShader(nullptr)
		, tileRenderer(nullptr)
		, player(nullptr)
		, camera(nullptr)
		, textRenderer(nullptr)
	{};
	virtual ~PlayScene();
};

