#include "UIManager.h"

#include "../GLFramework/UI/UI.h"
#include "HandUI.h"
#include "TextUI.h"

UIManager::UIManager()
	: currUI{ nullptr }
{

};

UIManager::~UIManager()
{

}

void UIManager::Init()
{
	handUI = new HandUI();
	handUI->Init();

	textUI = new TextUI();
	textUI->Init();
	textUI->SetText("TextUI Test");
	textUI->SetLocalPosition(glm::vec2{ 500.f, 500.f });
}

void UIManager::Release()
{
	SAFE_RELEASE(textUI);
	SAFE_RELEASE(handUI);

	for (auto& [key, ui] : UIs)
	{
		SAFE_RELEASE(ui);
	}
	UIs.clear();

	ReleaseSingleton();
}

void UIManager::Update()
{
	if (currUI)
		currUI->Update();

	handUI->Update();
	textUI->Update();
}

void UIManager::Render(ShaderProgram* shader)
{
	if (currUI)
		currUI->Render(shader);

	handUI->Render(shader);
	textUI->Render(shader);
}

void UIManager::AddUI(const std::string& key, UI* ui)
{
	UIs.emplace(key, ui);
}

void UIManager::ActiveUI(const std::string& key)
{
	DeactiveUI();

	if (UI* ui = FindUI(key))
	{
		currUI = ui;
		currUI->SetActive(true);
	}
}

void UIManager::DeactiveUI()
{
	if (currUI)
	{
		currUI->SetActive(false);
		currUI = nullptr;
	}
}

UI* UIManager::GetLpCurrUI()
{
	return currUI;
}

HandUI* UIManager::GetLpHandUI()
{
	return handUI;
}

bool UIManager::IsCurrUINull() const
{
	return currUI == nullptr;
}

bool UIManager::IsMouseOnUI() const
{
	if (currUI == nullptr)
	{
		return false;
	}

	return PtInFRect(currUI->GetFrect(), glm::vec2{ g_ptMouse.x, g_ptMouse.y });
}

UI* UIManager::FindUI(const std::string& key)
{
	return UIs.contains(key) ? UIs[key] : nullptr;
}
