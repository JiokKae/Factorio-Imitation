#pragma once
#include "framework.h"

class UI;
class Shader;
class HandUI;
class TextUI;
class UIManager : public Singleton<UIManager>
{
private:
	map<string, UI*> mapUIs;
	UI* currUI;

	HandUI* handUI;
	
	TextUI* textUI;
public:
	HRESULT Init();
	void Release();
	void Update();
	void HandUpdate();
	void Render(ShaderProgram* lpShader);

	UI* AddUI(string strKey, UI* lpUi);
	void DeleteUI(string strKey);
	UI* FindUI(string strKey);
	
	bool IsCurrUINull() { return currUI == nullptr; }
	bool IsMouseOnUI();
	void ActiveUI(string strKey);
	void DeactiveUI();

	UI* GetLpCurrUI()		{ return currUI; }
	HandUI* GetLpHandUI()	{ return handUI; }

	UIManager()
		: currUI(nullptr) 
	{};
};

