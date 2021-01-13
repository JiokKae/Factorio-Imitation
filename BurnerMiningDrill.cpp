#include "BurnerMiningDrill.h"
#include "Tile.h"
#include "Ore.h"
#include "ItemOnGround.h"
#include "BurnerMiningDrillUI.h"

HRESULT BurnerMiningDrill::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::BURNER_MINING_DRILL;
	usingClickEvent = true;
	Structure::Init(x, y, direction, temp);

	image = new GLImage[DIRECTION_END]();
	image[NORTH].Init(	"Entity/BurnerMiningDrill-N", 4, 8);
	image[EAST].Init(	"Entity/BurnerMiningDrill-E", 4, 8);
	image[SOUTH].Init(	"Entity/BurnerMiningDrill-S", 4, 8);
	image[WEST].Init(	"Entity/BurnerMiningDrill-W", 4, 8);

	shadow = new GLImage[DIRECTION_END]();
	shadow[NORTH].Init("Entity/BurnerMiningDrill-N-shadow", 4, 8);
	shadow[NORTH].SetAlpha(0.6f);
	shadowAniOffset[NORTH] = { 50, 0 };
	shadow[EAST].Init("Entity/BurnerMiningDrill-E-shadow", 4, 8);
	shadow[EAST].SetAlpha(0.6f);
	shadowAniOffset[EAST] = { 10, 0 };
	shadow[SOUTH].Init("Entity/BurnerMiningDrill-S-shadow", 4, 8);
	shadow[SOUTH].SetAlpha(0.6f);
	shadow[WEST].Init("Entity/BurnerMiningDrill-W-shadow", 4, 8);
	shadow[WEST].SetAlpha(0.6f);
	shadowAniOffset[WEST] = { 10, 0 };

	miningArea = glm::ivec2(2, 2);
	miningSpeed = 0.25f;
	energyConsumption = 150.0f;

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
	SAFE_ARR_RELEASE(image, 4);
	SAFE_ARR_RELEASE(shadow, 4);
}

void BurnerMiningDrill::Update()
{
	Structure::Update();

	if (temp)
		return;

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
		OutputItem();
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
			currPower -= energyConsumption * TimerManager::GetSingleton()->GetTimeElapsed();
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

bool BurnerMiningDrill::InputItem(ItemInfo* inputItem, glm::vec2 pos)
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
				SAFE_DELETE(inputItem);
				return true;
			}
		}
		// 연료 슬롯에 연료가 없다면
		else
		{
			waitingItemInfo->id = inputItem->id;
			waitingItemInfo->amount = inputItem->amount;
			SAFE_DELETE(inputItem);
			return true;
		}
	}

	SAFE_DELETE(inputItem);
	return false;
}

bool BurnerMiningDrill::OutputItem()
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
	glm::vec2 outputPos = {};

	if (outputTile->GetLpSturcture())
	{
		if (outputTile->GetLpSturcture()->InputItem(new ItemInfo(COAL, 1), glm::vec2(0.0f)))
		{
			targetTile->GetLpOre()->AddAmount(-1);
			productionPercent -= 1.0f;
			if (IsMiningAreaEmpty())
			{
				status = NO_MINABLE_RESOURCES;
				return false;
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
		glm::vec2 outTilePos = glm::vec2(COORD_TO_POS(outCoord));
		targetTile->GetLpOre()->AddAmount(-1);

		ItemOnGround* item = new ItemOnGround();
		item->Init(ItemEnum::COAL);
		item->SetPosition(outTilePos + outputPos);
		EntityManager::GetSingleton()->AddItemOnGround(item);

		productionPercent -= 1.0f;

		if (IsMiningAreaEmpty())
		{
			status = NO_MINABLE_RESOURCES;
			return false;
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

	return false;
}

void BurnerMiningDrill::ClickEvent()
{
	UIManager::GetSingleton()->ActiveUI("BurnerMiningDrillUI");
	BurnerMiningDrillUI* bmdUI = (BurnerMiningDrillUI*)UIManager::GetSingleton()->GetLpCurrUI();
	bmdUI->SetCurrBurnerMiningDrill(this);
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
