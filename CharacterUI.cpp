#include "CharacterUI.h"
#include "GLImage.h"

HRESULT CharacterUI::Init()
{
	image = new GLImage();
	image->Init("UI/CharacterUI", 884, 500);

	return S_OK;
}

void CharacterUI::Release()
{
}

void CharacterUI::Update()
{
	if (active)
	{
		;
	}
}

void CharacterUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, 0, 0);
	}
}
