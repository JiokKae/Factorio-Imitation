#include "BurnerMiningDrill.h"
#include "GLImage.h"

HRESULT BurnerMiningDrill::Init()
{
	image = new GLImage[int(DIRECTION::END)]();
	image[int(DIRECTION::NORTH)].Init("Burner-mining-drill/hr-burner-mining-drill-N", 692, 1504, 4, 8);
	shadow = new GLImage[int(DIRECTION::END)]();

	return S_OK;
}

void BurnerMiningDrill::Release()
{
	for (int i = 0; i < 4; i++)
		image->Release();
	SAFE_ARR_DELETE(image);

	for (int i = 0; i < 4; i++)
		shadow->Release();
	SAFE_ARR_DELETE(shadow);
}

void BurnerMiningDrill::Update()
{
}

void BurnerMiningDrill::Render(Shader* lpShader)
{
}
