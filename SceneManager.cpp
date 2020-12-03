#include "SceneManager.h"
#include "GameNode.h"

DWORD CALLBACK LoadingThread(LPVOID pvParam);

GameNode* SceneManager::currScene = nullptr;
GameNode* SceneManager::loadingScene = nullptr;
GameNode* SceneManager::readyScene = nullptr;

HRESULT SceneManager::Init()
{
	return S_OK;
}

void SceneManager::Release()
{
	map<string, GameNode*>::iterator it;
	for (it = mapSceneData.begin(); it != mapSceneData.end(); )
	{
		if (it->second)
		{
			(it->second)->Release();
			delete (it->second);
			it = mapSceneData.erase(it);
		}
		else
		{
			it++;
		}
	}
	mapSceneData.clear();

	for (it = mapLoadingSceneData.begin(); it != mapLoadingSceneData.end(); )
	{
		if (it->second)
		{
			(it->second)->Release();
			delete (it->second);
			it = mapLoadingSceneData.erase(it);
		}
		else
		{
			it++;
		}
	}
	mapLoadingSceneData.clear();
	ReleaseSingleton();
}

void SceneManager::Update()
{
	if(currScene)	
		currScene->Update();
}

void SceneManager::Render(HDC hdc)
{
	if (currScene)
		currScene->Render(hdc);
}

GameNode * SceneManager::AddScene(string key, GameNode * scene)
{
	if (scene == nullptr)
		return nullptr;

	mapSceneData.insert(pair<string, GameNode*>(key, scene));

	return scene;
}

GameNode * SceneManager::AddLoadingScene(string key, GameNode * scene)
{
	if (scene == nullptr)
		return nullptr;

	mapLoadingSceneData.insert(pair<string, GameNode*>(key, scene));

	return scene;
}

HRESULT SceneManager::ChangeScene(string sceneName)
{
	map<string, GameNode*>::iterator it = mapSceneData.find(sceneName);
	if (it == mapSceneData.end())
	{
		return E_FAIL;
	}

	if (SUCCEEDED(it->second->Init()))
	{
		if (currScene)
		{
			currScene->Release();
		}
		currScene = it->second;
		return S_OK;
	}

	return E_FAIL;
}

HRESULT SceneManager::ChangeScene(string sceneName, string loadingSceneName)
{
	map<string, GameNode*>::iterator it = mapSceneData.find(sceneName);
	if (it == mapSceneData.end())
		return E_FAIL;

	map<string, GameNode*>::iterator itLoading = mapLoadingSceneData.find(loadingSceneName);
	if (itLoading == mapLoadingSceneData.end())
		return ChangeScene(sceneName);

	if (SUCCEEDED(itLoading->second->Init()))
	{
		if (currScene)
		{
			currScene->Release();
		}

		readyScene = it->second;
		loadingScene = itLoading->second;

		currScene = loadingScene;

		HANDLE hThread;
		DWORD loadThreadID;
		hThread = CreateThread(NULL, 0, LoadingThread, NULL, 0, &loadThreadID);

		CloseHandle(hThread);

		return S_OK;
	}

	return E_FAIL;
}

DWORD CALLBACK LoadingThread(LPVOID pvParam)
{
	SceneManager::readyScene->Init();
	SceneManager::currScene = SceneManager::readyScene;

	SceneManager::loadingScene->Release();
	SceneManager::loadingScene = nullptr;
	SceneManager::readyScene = nullptr;

	return 0;
}
