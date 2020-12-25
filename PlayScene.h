#pragma once
#include "Scene.h"

class Camera;
class Shader;
class Character;
class CharacterUI;
class TileRenderer;
class PlayScene : public Scene
{
	Camera* camera;
	Shader* lightingShader;
	Shader* UIShader;
	TileRenderer* tileRenderer;
	Character* player;
	CharacterUI* characterUI;

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
	{};
	virtual ~PlayScene();
};

