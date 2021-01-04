#include "BurnerMiningDrill.h"
#include "GLImage.h"

HRESULT BurnerMiningDrill::Init(int x, int y)
{
	position = { x, y };
	size = { 2, 2 };
	image = new GLImage[int(DIRECTION::END)]();
	image[DIRECTION::NORTH].Init("Burner-mining-drill/hr-burner-mining-drill-N", 4, 8);
	image[DIRECTION::EAST].Init("Burner-mining-drill/hr-burner-mining-drill-E", 4, 8);
	image[DIRECTION::SOUTH].Init("Burner-mining-drill/hr-burner-mining-drill-S", 4, 8);
	image[DIRECTION::WEST].Init("Burner-mining-drill/hr-burner-mining-drill-W", 4, 8);

	shadow = new GLImage[int(DIRECTION::END)]();
	shadow[DIRECTION::NORTH].Init("Burner-mining-drill/hr-burner-mining-drill-N-shadow", 4, 8);
	shadow[DIRECTION::NORTH].SetAlpha(0.6f);
	shadowAniOffset[DIRECTION::NORTH] = {50, 0};
	shadow[DIRECTION::EAST].Init("Burner-mining-drill/hr-burner-mining-drill-E-shadow", 4, 8);
	shadow[DIRECTION::EAST].SetAlpha(0.6f);
	shadow[DIRECTION::SOUTH].Init("Burner-mining-drill/hr-burner-mining-drill-S-shadow", 4, 8);
	shadow[DIRECTION::SOUTH].SetAlpha(0.6f);
	shadow[DIRECTION::WEST].Init("Burner-mining-drill/hr-burner-mining-drill-W-shadow", 4, 8);
	shadow[DIRECTION::WEST].SetAlpha(0.6f);

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
	if (PtInFRect(GetFRect(), g_cursorPosition))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			UIManager::GetSingleton()->ActiveUI("BurnerMiningDrillUI");
		}

		if (KeyManager::GetSingleton()->IsOnceKeyDown('R'))
		{
			SoundManager::GetSingleton()->Play("RotateMedium", 0.6f);
			direction = DIRECTION(direction + 1);
			if (direction == DIRECTION::END)
				direction = DIRECTION::NORTH;
		}
	}
}

void BurnerMiningDrill::Render(Shader* lpShader)
{
	shadow[direction].Render(lpShader, position.x + shadowAniOffset[direction].x, position.y + shadowAniOffset[direction].y, 0, 0);
	image[direction].Render(lpShader, position.x, position.y, 0, 0);
}

FRECT BurnerMiningDrill::GetFRect()
{
	FRECT rect;
	rect.left =		position.x - (size.x * TILE_SIZE / 2.0f);
	rect.right =	position.x + (size.x * TILE_SIZE / 2.0f);
	rect.top =		position.y + (size.y * TILE_SIZE / 2.0f);
	rect.bottom =	position.y - (size.y * TILE_SIZE / 2.0f);
	return rect;
}

FRECT BurnerMiningDrill::GetCollisionFRect()
{
	FRECT rect;
	rect.left =		position.x - (size.x * TILE_SIZE / 2.0f * (2.0f / 3.0f));
	rect.right =	position.x + (size.x * TILE_SIZE / 2.0f * (2.0f / 3.0f));
	rect.top =		position.y + (size.y * TILE_SIZE / 2.0f * (2.0f / 3.0f));
	rect.bottom =	position.y - (size.y * TILE_SIZE / 2.0f * (2.0f / 3.0f));
	return rect;
}
