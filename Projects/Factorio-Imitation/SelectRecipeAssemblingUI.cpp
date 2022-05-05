#include "SelectRecipeAssemblingUI.h"
#include "DeactiveButtonUI.h"
#include "GLFramework/Image/GLImage.h"

HRESULT SelectRecipeAssemblingUI::Init()
{
	image = new GLImage();
	image->Init("UI/SelectRecipeAssemblingUI");

	deactiveButtonUI = new DeactiveButtonUI();
	deactiveButtonUI->Init();
	deactiveButtonUI->SetParent(this);
	deactiveButtonUI->SetLocalPosition(glm::vec2(200, 230));

	return S_OK;
}

void SelectRecipeAssemblingUI::Release()
{
	SAFE_RELEASE(deactiveButtonUI);
	SAFE_RELEASE(image);
}

void SelectRecipeAssemblingUI::Update()
{
	if (active)
	{
		deactiveButtonUI->Update();
	}
}

void SelectRecipeAssemblingUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y);
		deactiveButtonUI->Render(lpShader);
	}
}
