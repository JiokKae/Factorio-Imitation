#pragma once
#include "Scene.h"

class Camera;
class Shader;
class Character;
class PointLight;
class TileManager;
class TextRenderer;
class DirectionalLight;
class EntityManager;
class StructureBuilder;
class PlayScene : public Scene
{
	Camera* camera;
	Shader* lightingShader;
	GLuint uboMatrices;
	GLuint uboLights;
	GLuint uboUIMatrices;

	Shader* UIShader;
	TileManager* tileRenderer;
	Character* player;

	TextRenderer* textRenderer;

	DirectionalLight* dirLight;
	PointLight* pointLights;
	int numOfPointLight;

	EntityManager* entityManager;

	StructureBuilder* structureBuilder;
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
		, entityManager(nullptr)
	{};
	virtual ~PlayScene();
};

