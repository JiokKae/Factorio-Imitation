#include "UIManager.h"
#include "UI.h"
#include "HandUI.h"
HRESULT UIManager::Init()
{
	handUI = new HandUI();

	return S_OK;
}

void UIManager::Release()
{
	SAFE_RELEASE(handUI);

	map<string, UI*>::iterator it;
	for (it = mapUIs.begin(); it != mapUIs.end(); )
	{
		if (it->second)
		{
			(it->second)->Release();
			delete (it->second);
			it = mapUIs.erase(it);
		}
		else
		{
			it++;
		}
	}
	mapUIs.clear();

	ReleaseSingleton();
}

void UIManager::Update()
{
	if (currUI)
		currUI->Update();

	handUI->Update();
}

void UIManager::Render(Shader* lpShader)
{
	if(currUI)
		currUI->Render(lpShader);

	handUI->Render(lpShader);
}

UI* UIManager::AddUI(string strKey, UI* lpUi)
{
	UI* ui = FindUI(strKey);

	if (ui)
	{
		return ui;
	}

	mapUIs.insert(make_pair(strKey, lpUi));

	return lpUi;
}

void UIManager::DeleteUI(string strKey)
{
	map<string, UI*>::iterator it = mapUIs.find(strKey);
	if (it != mapUIs.end())
	{
		it->second->Release();
		delete it->second;

		mapUIs.erase(it);
	}
}

UI* UIManager::FindUI(string strKey)
{
	map<string, UI*>::iterator it = mapUIs.find(strKey);
	if (it != mapUIs.end())
	{
		return it->second;
	}

	return nullptr;
}

void UIManager::ActiveUI(string strKey)
{
	if(currUI)
		currUI->SetActive(false);

	UI* ui = FindUI(strKey);

	currUI = ui;
	currUI->SetActive(true);
}

void UIManager::DeactiveUI()
{
	if (currUI)
		currUI->SetActive(false);
	currUI = nullptr;
}
