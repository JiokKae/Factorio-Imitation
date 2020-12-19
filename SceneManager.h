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
	static GameNode* currScene;		// ���� ��� ���� ��
	static GameNode* loadingScene;	// ���� �� �غ� �߿� ��µ� ��
	static GameNode* readyScene;	// �غ� ���� ��

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	// �� �߰�
	GameNode* AddScene(string key, GameNode* scene);
	GameNode* AddLoadingScene(string key, GameNode* scene);

	// �� ����
	HRESULT ChangeScene(string sceneName);
	HRESULT ChangeScene(string sceneName, string loadingSceneName);

	virtual ~SceneManager() {};
};


