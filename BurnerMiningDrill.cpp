#include "BurnerMiningDrill.h"
#include "Tile.h"
#include "Ore.h"
#include "ItemOnGround.h"
#include "BurnerMiningDrillUI.h"

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

	miningArea = glm::ivec2(2, 2);
	miningPower = 150.0f;
	miningSpeed = 0.25f;

	// ≈∏∞Ÿ ≈∏¿œ º≥¡§
	miningAreaTiles = new Tile*[4]();
	for (int y = 0; y < miningArea.y; y++)
	{
		for (int x = 0; x < miningArea.x; x++)
		{
			Tile* tile = TileManager::GetSingleton()->GetLpTile( coord.x - coordSize.x / 2 + x,	coord.y - coordSize.y / 2 + y);
			miningAreaTiles[y * miningArea.x + x] = tile;
		}
	}

	for (int i = 0; i < miningArea.x * miningArea.y; i++)
	{
		if (miningAreaTiles[i]->GetLpOre()->GetAmount() > 0)
		{
			targetTile = miningAreaTiles[i];
			break;
		}
	}
	if (targetTile == nullptr)
		status = NO_MINABLE_RESOURCES;

	waitingItemInfo = new ItemInfo(ItemEnum::COAL, 1);


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
	Structure::Update();

	if (PtInFRect(GetFRect(), g_cursorPosition))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			UIManager::GetSingleton()->ActiveUI("BurnerMiningDrillUI");
			BurnerMiningDrillUI* bmdUI = (BurnerMiningDrillUI*)UIManager::GetSingleton()->GetLpCurrUI();
			bmdUI->SetCurrBurnerMiningDrill(this);
		}
	}

	switch (status)
	{
	case BurnerMiningDrill::NOPOWER:
		if (waitingItemInfo)
		{
			if (waitingItemInfo->amount > 0)
			{
				waitingItemInfo->amount--;
				currPower += 2000;
				maxPower = 2000;
				status = WORKING;
			}
		}	
		break;

	case BurnerMiningDrill::WAITING_SPACE:
		break;

	case BurnerMiningDrill::WORKING:
		if (currPower <= 0)
		{
			status = NOPOWER;
			break;
		}
		else
		{
			currPower -= miningPower * TimerManager::GetSingleton()->GetTimeElapsed();
			productionPercent += miningSpeed * TimerManager::GetSingleton()->GetTimeElapsed();

			// √§±§ øœ∑·
			if (productionPercent >= 1.0f)
			{
				targetTile->GetLpOre()->AddAmount(-1);
				ItemOnGround* item = new ItemOnGround();
				item->Init(ItemEnum::COAL);
				item->SetPosition(Vec2(position.x + 32, position.y - 80));
				EntityManager::GetSingleton()->AddItemOnGround(item);
				productionPercent -= 1.0f;

				if (IsMiningAreaEmpty())
				{
					status = NO_MINABLE_RESOURCES;
					break;
				}

				for (int i = 0; i < miningArea.length(); i++)
				{
					if (miningAreaTiles[i]->GetLpOre()->GetAmount() > 0)
					{
						targetTile = miningAreaTiles[i];
						break;
					}
				}
			}

			time += TimerManager::GetSingleton()->GetTimeElapsed();
		}
		break;

	case BurnerMiningDrill::NO_MINABLE_RESOURCES:
		break;
	case BurnerMiningDrill::DESTORY:
		break;

	}

}

void BurnerMiningDrill::FirstRender(Shader* lpShader)
{
	glm::ivec2 maxFrame = image->GetMaxFrame();
	int frame = abs(int(time * 30) % (maxFrame.x * maxFrame.y * 2 - 1) - maxFrame.x * maxFrame.y + 1);

	int frameX = frame % maxFrame.x;
	int frameY = maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y;

	shadow[direction].Render(lpShader, position.x + shadowAniOffset[direction].x, position.y + shadowAniOffset[direction].y,
		frameX, frameY);
}

void BurnerMiningDrill::Render(Shader* lpShader)
{
	glm::ivec2 maxFrame = image->GetMaxFrame();
	int frame = abs(int(time * 30) % (maxFrame.x * maxFrame.y * 2 - 1) - maxFrame.x * maxFrame.y + 1);

	int frameX = frame % maxFrame.x;
	int frameY = maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y;

	image[direction].Render(lpShader, position.x, position.y,
		frameX, frameY);
}

bool BurnerMiningDrill::IsMiningAreaEmpty()
{
	for (int i = 0; i < miningArea.x * miningArea.y; i++)
	{
		if (miningAreaTiles[i]->GetLpOre()->GetAmount() > 0)
			return false;
	}
	return true;
}
