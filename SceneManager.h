#pragma once
#include "framework.h"
#include "Singleton.h"

class Image;
class GameNode;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<string, GameNode*> mapSceneData;
	map<string, GameNode*> mapLoadingSceneData;

	HDC backDC;
	Image* backBuffer;
	
public:
	static GameNode* currScene;		// 현재 출력 중인 씬
	static GameNode* loadingScene;	// 다음 씬 준비 중에 출력될 씬
	static GameNode* readyScene;	// 준비 중인 씬

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	// 씬 추가
	GameNode* AddScene(string key, GameNode* scene);
	GameNode* AddLoadingScene(string key, GameNode* scene);

	// 씬 변경
	HRESULT ChangeScene(string sceneName);
	HRESULT ChangeScene(string sceneName, string loadingSceneName);

	virtual ~SceneManager() {};
};


