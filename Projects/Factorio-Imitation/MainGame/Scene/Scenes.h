#pragma once

#include "2DFramework/Scene/Scene.h"
#include "../../framework.h"

class FreeCamera;
class Texture;
class Shader;
class Camera;
class PointLight;
class TileManager;
class TextRenderer;
class DirectionalLight;
class DebugHelper;
class Image;
class Button;

class LightingScene : public Scene
{
	FreeCamera* camera;
	ShaderProgram* lightingShader;
	ShaderProgram* lightCubeShader;

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

class LoadingScene1 : public Scene
{
private:
	Image* img;

public:
	virtual HRESULT Init()		 override;
	virtual void Release()		 override;
	virtual void Update()		 override;
	virtual void Render(HDC hdc) override;

	LoadingScene1(int width, int height)
		: Scene(width, height)
		, img(nullptr)
	{};
	virtual ~LoadingScene1() {};
};

class PlayScene : public Scene
{
	Camera* camera;
	ShaderProgram* lightingShader;
	GLuint uboMatrices;
	GLuint uboLights;
	GLuint uboUIMatrices;

	ShaderProgram* UIShader;
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

class TenCubeSpaceScene : public Scene
{
private:
	int Width, Height;
	GLint EnvMode = GL_REPLACE;
	GLint TexFilter = GL_LINEAR;

	unsigned int VBO;
	unsigned int VAO;

	Texture* texture1;
	Texture* texture2;
	float radio = 0.5;

	ShaderProgram* ourShader;
	FreeCamera* camera;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	TenCubeSpaceScene(int width, int height)
		: Scene(width, height)
		, ourShader(nullptr)
		, camera(nullptr)
	{};

	virtual ~TenCubeSpaceScene();
};

#define	SAMPLE_TILE_X	20
#define	SAMPLE_TILE_Y	9

#define	TILE_X		20
#define	TILE_Y		20

enum class TERRAIN { WALL, GRASS, EARTH, WATER, CEMENT, END };

struct TILE_INFO
{
	TERRAIN terrian;
	RECT rc;
	int frameX;
	int frameY;
};

typedef struct tagSampleTile
{
	RECT rc;
	int frameX;
	int frameY;
} SAMPLE_TILE_INFO;

typedef struct tagSelectTile
{
	RECT frame;
} SELECT_TILE_INFO;

class TileMapToolScene : public Scene
{
private:
	Image* sampleTile;
	Image* selectTileEdge;
	// 메인 타일의 정보
	RECT rcMain;
	TILE_INFO tileInfo[TILE_X * TILE_Y];

	// 샘플 타일의 정보
	RECT rcSample;
	SAMPLE_TILE_INFO sampleTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];

	// 선택된 타일의 정보
	SELECT_TILE_INFO selectTileInfo;
	POINT selectStartFrame;
	bool sampleSelectStart;

	// 세이브 로드 버튼
	Button* saveButton;
	Button* loadButton;
public:
	virtual HRESULT Init()		 override;
	virtual void Release()		 override;
	virtual void Update()		 override;
	virtual void Render(HDC hdc) override;

	TileMapToolScene(int width, int height)
		: Scene(width, height)
		, sampleSelectStart(false)
	{};
	virtual ~TileMapToolScene() {};
};

class TitleScene : public Scene
{
private:
	Image* img;
	Image* mainMenuImage;

	Button* button1;
	Button* gameStartButton;
	Button* TenCubeButton;
	Button* quitButton;
	Button* lightingButton;
public:
	virtual HRESULT Init()		override;
	virtual void Release()		override;
	virtual void Update()		override;
	virtual void Render(HDC hdc)	override;

	TitleScene(int width, int height)
		: Scene(width, height)
	{};
	virtual ~TitleScene() {};
};
