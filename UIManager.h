#pragma once
#include "Singleton.h"
#include "framework.h"

class UI;
class Shader;
class UIManager : public Singleton<UIManager>
{
private:
	map<string, UI*> mapUIs;
	UI* currUI;
	
public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(Shader* lpShader);

	UI* AddUI(string strKey, UI* lpUi);
	void DeleteUI(string strKey);
	UI* FindUI(string strKey);
	
	bool IsCurrUINull() { return currUI == nullptr; }
	void ActiveUI(string strKey);
	void DeactiveUI();

	UIManager()
		: currUI(nullptr) 
	{};
};

