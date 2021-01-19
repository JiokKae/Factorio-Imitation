#pragma once
#include "framework.h"
#include "Singleton.h"

class Image;
class Scene;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<string, Scene*> mapSceneData;
	map<string, Scene*> mapLoadingSceneData;

	HDC backDC;
	Image* backBuffer;
	
public:
	static Scene* currScene;	// 현재 출력 중인 씬
	static Scene* loadingScene;	// 다음 씬 준비 중에 출력될 씬
	static Scene* readyScene;	// 준비 중인 씬

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	// 씬 추가
	Scene* AddScene(string key, Scene* scene);
	Scene* AddLoadingScene(string key, Scene* scene);

	// 씬 변경
	HRESULT ChangeScene(string sceneName);
	HRESULT ChangeScene(string sceneName, string loadingSceneName);

	virtual ~SceneManager() {};
};


