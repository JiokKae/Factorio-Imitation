#include "BurnerMiningDrillUI.h"
#include "GLImage.h"

HRESULT BurnerMiningDrillUI::Init()
{
	image = new GLImage();
	image->Init("UI/BunerMiningDrillUI");

	return S_OK;
}

void BurnerMiningDrillUI::Release()
{
}

void BurnerMiningDrillUI::Update()
{
}

void BurnerMiningDrillUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y);
	}
}
