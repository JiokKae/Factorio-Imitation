#include "BurnerMiningDrill.h"
#include "GLImage.h"

HRESULT BurnerMiningDrill::Init(int x, int y)
{
	itemId = ItemEnum::BURNER_MINING_DRILL;
	Structure::Init(x, y);

	image = new GLImage[DIRECTION_END]();
	image[DIRECTION::NORTH].Init(	"Entity/BurnerMiningDrill-N", 4, 8);
	image[DIRECTION::EAST].Init(	"Entity/BurnerMiningDrill-E", 4, 8);
	image[DIRECTION::SOUTH].Init(	"Entity/BurnerMiningDrill-S", 4, 8);
	image[DIRECTION::WEST].Init(	"Entity/BurnerMiningDrill-W", 4, 8);

	shadow = new GLImage[DIRECTION_END]();
	shadow[DIRECTION::NORTH].Init("Entity/BurnerMiningDrill-N-shadow", 4, 8);
	shadow[DIRECTION::NORTH].SetAlpha(0.6f);
	shadowAniOffset[DIRECTION::NORTH] = {50, 0};
	shadow[DIRECTION::EAST].Init("Entity/BurnerMiningDrill-E-shadow", 4, 8);
	shadow[DIRECTION::EAST].SetAlpha(0.6f);
	shadow[DIRECTION::SOUTH].Init("Entity/BurnerMiningDrill-S-shadow", 4, 8);
	shadow[DIRECTION::SOUTH].SetAlpha(0.6f);
	shadow[DIRECTION::WEST].Init("Entity/BurnerMiningDrill-W-shadow", 4, 8);
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
			SoundManager::GetSingleton()->Play("Rotate-medium", 0.6f);
			direction = DIRECTION(direction + 1);
			if (direction == DIRECTION_END)
				direction = DIRECTION::NORTH;
		}
	}
}

void BurnerMiningDrill::Render(Shader* lpShader)
{
	static int oframe;
	int frame = oframe/600;
	glm::ivec2 maxFrame = image->GetMaxFrame();
	shadow[direction].Render(lpShader, position.x + shadowAniOffset[direction].x, position.y + shadowAniOffset[direction].y,
		maxFrame.x - 1 - frame % maxFrame.x, frame / maxFrame.x % maxFrame.y);
	image[direction].Render(lpShader, position.x, position.y,
		maxFrame.x - 1 - frame % maxFrame.x, frame / maxFrame.x % maxFrame.y);
	oframe++;
}
