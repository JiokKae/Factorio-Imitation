#include "BurnerMiningDrill.h"
#include "GLImage.h"

HRESULT BurnerMiningDrill::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::BURNER_MINING_DRILL;
	Structure::Init(x, y, direction, temp);

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

void BurnerMiningDrill::FirstRender(Shader* lpShader)
{
	glm::ivec2 maxFrame = image->GetMaxFrame();
	int frame = abs(int(g_time * 30) % (maxFrame.x * maxFrame.y * 2 - 1) - maxFrame.x * maxFrame.y + 1);

	int frameX = frame % maxFrame.x;
	int frameY = maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y;

	shadow[direction].Render(lpShader, position.x + shadowAniOffset[direction].x, position.y + shadowAniOffset[direction].y,
		frameX, frameY);
}

void BurnerMiningDrill::Render(Shader* lpShader)
{
	glm::ivec2 maxFrame = image->GetMaxFrame();
	int frame = abs(int(g_time * 30) % (maxFrame.x * maxFrame.y * 2 - 1) - maxFrame.x * maxFrame.y + 1);

	int frameX = frame % maxFrame.x;
	int frameY = maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y;

	image[direction].Render(lpShader, position.x, position.y,
		frameX, frameY);
}
