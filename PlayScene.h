#pragma once
#include "Scene.h"

class Camera;
class Shader;
class GLImage;
class Character;
class CharacterUI;
class Tile;
class PlayScene : public Scene
{
	Camera* camera;
	Shader* lightingShader;
	Shader* UIShader;
	Tile* tiles;
	Character* player;
	GLImage* dirt_1Image;

	CharacterUI* characterUI;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	PlayScene(int width, int height)
		: Scene(width, height)
		, camera(nullptr)
		, lightingShader(nullptr)
		, player(nullptr)
		, dirt_1Image(nullptr)
	{};
	virtual ~PlayScene();
};

