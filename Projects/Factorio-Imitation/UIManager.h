#pragma once

#include "framework.h"
#include <map>
#include <string>

class HandUI;
class ShaderProgram;
class TextUI;
class UI;

class UIManager : public Singleton<UIManager>
{
public:
	UIManager();
	~UIManager();

	void Init();
	void Release();
	void Update();
	void Render(ShaderProgram* shader);

	void AddUI(const std::string& key, UI* ui);
	void ActiveUI(const std::string& key);
	void DeactiveUI();

	UI* GetLpCurrUI();
	HandUI* GetLpHandUI();

	bool IsCurrUINull() const;
	bool IsMouseOnUI() const;

private:
	UI* FindUI(const std::string& key);

	std::map<std::string, UI*> UIs;
	UI* currUI;

	HandUI* handUI;
	TextUI* textUI;
};
