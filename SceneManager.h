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
	static Scene* currScene;	// ���� ��� ���� ��
	static Scene* loadingScene;	// ���� �� �غ� �߿� ��µ� ��
	static Scene* readyScene;	// �غ� ���� ��

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	// �� �߰�
	Scene* AddScene(string key, Scene* scene);
	Scene* AddLoadingScene(string key, Scene* scene);

	// �� ����
	HRESULT ChangeScene(string sceneName);
	HRESULT ChangeScene(string sceneName, string loadingSceneName);

	virtual ~SceneManager() {};
};


