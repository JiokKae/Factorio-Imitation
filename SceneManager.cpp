#include "SceneManager.h"
#include "Scene.h"
#include "Image.h"

DWORD CALLBACK LoadingThread(LPVOID pvParam);

Scene* SceneManager::currScene = nullptr;
Scene* SceneManager::loadingScene = nullptr;
Scene* SceneManager::readyScene = nullptr;

HRESULT SceneManager::Init()
{
	backBuffer = new Image();
	backBuffer->Init(1920, 1080);
	backDC = backBuffer->GetMemDC();

	return S_OK;
}

void SceneManager::Release()
{
	map<string, Scene*>::iterator it;
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

	SAFE_RELEASE(backBuffer);

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
	{
		if (((Scene*)currScene)->IsUseBackBuffer())
		{
			currScene->Render(backDC);
			TimerManager::GetSingleton()->Render(backDC);
			// 백버퍼 복사(출력)
			backBuffer->Render(hdc, 0, 0, WINSIZE_X, WINSIZE_Y);
		}
		else
		{
			currScene->Render(hdc);
		}
	}
		
}

Scene* SceneManager::AddScene(string key, Scene* scene)
{
	if (scene == nullptr)
		return nullptr;

	mapSceneData.insert(pair<string, Scene*>(key, scene));

	return scene;
}

Scene* SceneManager::AddLoadingScene(string key, Scene* scene)
{
	if (scene == nullptr)
		return nullptr;

	mapLoadingSceneData.insert(pair<string, Scene*>(key, scene));

	return scene;
}

HRESULT SceneManager::ChangeScene(string sceneName)
{
	map<string, Scene*>::iterator it = mapSceneData.find(sceneName);
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
	map<string, Scene*>::iterator it = mapSceneData.find(sceneName);
	if (it == mapSceneData.end())
		return E_FAIL;

	map<string, Scene*>::iterator itLoading = mapLoadingSceneData.find(loadingSceneName);
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
