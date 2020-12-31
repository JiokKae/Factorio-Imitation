#include "BurnerMiningDrill.h"
#include "GLImage.h"

HRESULT BurnerMiningDrill::Init(int x, int y)
{
	leftTopcoord = { x, y };
	image = new GLImage[int(DIRECTION::END)]();
	image[int(DIRECTION::NORTH)].Init("Burner-mining-drill/hr-burner-mining-drill-N", 4, 8);
	shadow = new GLImage[int(DIRECTION::END)]();
	shadow[int(DIRECTION::NORTH)].Init("Burner-mining-drill/hr-burner-mining-drill-N-shadow", 4, 8);
	shadow->SetAlpha(0.6f);
	
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
	shadow[int(DIRECTION::NORTH)].Render(lpShader, leftTopcoord.x, leftTopcoord.y, 0, 0);
	image[int(DIRECTION::NORTH)].Render(lpShader, leftTopcoord.x, leftTopcoord.y, 0, 0);
}
