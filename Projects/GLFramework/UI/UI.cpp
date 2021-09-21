#include "UI.h"
#include "../Image/GLImage.h"

HRESULT UI::Init()
{
	return E_NOTIMPL;
}

void UI::Release()
{
}

void UI::Update()
{
}

void UI::Render(Shader* /*lpShader*/)
{
}

float UI::GetWidth()
{
	return image->GetFrameWidth();
}

float UI::GetHeight()
{
	return image->GetFrameHeight();
}

FRECT UI::GetFrect()
{
	FRECT rect;
	
	rect.left =		GetPosition().x - GetWidth() / 2;
	rect.right =	GetPosition().x + GetWidth() / 2;
	rect.top =		GetPosition().y + GetHeight() / 2;
	rect.bottom =	GetPosition().y - GetHeight() / 2;

	return rect;
}
