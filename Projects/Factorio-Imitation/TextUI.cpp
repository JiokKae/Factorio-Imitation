#include "TextUI.h"
#include "GLFramework/TextRenderer/TextRenderer.h"

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

void TextUI::Render(ShaderProgram* /*lpShaderProgram*/)
{
	TextRenderer::GetSingleton()->RenderText(text, GetPosition().x, GetPosition().y, 1.0f, glm::vec3(1.0f));
}
