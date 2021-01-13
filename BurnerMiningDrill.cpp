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

	// 타겟 타일 설정
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

	waitingItemInfo = new ItemInfo(ItemEnum::COAL, 0);


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

	if (temp)
		return;

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
				currPower += g_itemSpecs[waitingItemInfo->id].fuelValue;
				maxPower = g_itemSpecs[waitingItemInfo->id].fuelValue;
				status = WORKING;
			}
		}	
		break;

	case BurnerMiningDrill::WAITING_SPACE:
	{
		Tile* outputTile = nullptr;
		switch (direction)
		{
		case NORTH:
			outputTile = miningAreaTiles[2]->GetAroundTile(direction);
			break;
		case EAST:
			outputTile = miningAreaTiles[3]->GetAroundTile(direction);
			break;
		case SOUTH:
			outputTile = miningAreaTiles[1]->GetAroundTile(direction);
			break;
		case WEST:
			outputTile = miningAreaTiles[0]->GetAroundTile(direction);
			break;
		}
		if (outputTile->GetLpSturcture())
		{
			if (outputTile->GetLpSturcture()->InputItem(new ItemInfo(COAL, 1)))
			{
				targetTile->GetLpOre()->AddAmount(-1);
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
				status = WORKING;
			}
		}
		else
		{
			glm::ivec2 outCoord = outputTile->GetCoord();
			glm::vec2 outPos = glm::vec2(COORD_TO_POS(outCoord));
			targetTile->GetLpOre()->AddAmount(-1);
			ItemOnGround* item = new ItemOnGround();
			item->Init(ItemEnum::COAL);

			item->SetPosition(outPos);

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
			status = WORKING;
		}
		break;
	}

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

			// 채광 완료
			if (productionPercent >= 1.0f)
			{
				status = WAITING_SPACE;
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

bool BurnerMiningDrill::InputItem(ItemInfo* inputItem)
{
	// 받는 아이템이 연료라면
	if (g_itemSpecs[inputItem->id].fuel)
	{
		// 연료 슬롯에 연료가 있다면
		if (waitingItemInfo->amount)
		{
			// 연료 종류가 같을 때만
			if (waitingItemInfo->id == inputItem->id)
			{
				waitingItemInfo->amount += inputItem->amount;
				return true;
			}
		}
		// 연료 슬롯에 연료가 없다면
		else
		{
			waitingItemInfo->id = inputItem->id;
			waitingItemInfo->amount = inputItem->amount;
			return true;
		}
	}

	return false;
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
