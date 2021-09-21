#include "TextUI.h"

HRESULT TextUI::Init()
{
	return S_OK;
}

void TextUI::Release()
{
}

void TextUI::Update()
{
}

void TextUI::Render(Shader* lpShader)
{
	TextRenderer::GetSingleton()->RenderText(text, GetPosition().x, GetPosition().y, 1.0f, glm::vec3(1.0f));
}
