#include "Ore.h"
#include "ItemId.h"
#include "framework.h"
#include <array>

Ore::Ore(int itemEnum, int amount)
	: itemEnum{ itemEnum }
	, amount{ amount }
	, randFrameX{ std::rand() % 8 }
{
}

int Ore::GetItemEnum() const
{
	return itemEnum;
}

int Ore::GetAmount() const
{
	return amount;
}

int Ore::GetRandFrameX() const
{
	return randFrameX;
}

int Ore::GetFrameY() const
{
	constexpr std::array<int, 7> AMOUNTS{ 10000, 8000, 6000, 4000, 1500, 300, 200 };

	for (int i = 0; i < AMOUNTS.size(); ++i)
	{
		if (amount > AMOUNTS[i])
		{
			return static_cast<int>(AMOUNTS.size()) - i;
		}
	}
	return 0;
}

void Ore::AddAmount(int value)
{
	this->amount += value;
}

int Ore::RandomAmount(int x, int y)
{
	return std::max(0, int(10.0f - glm::distance(glm::vec2(16, 16), glm::vec2(std::abs(x % 32), std::abs(y % 32)))) * (300 + std::rand() % 200));
}

int Ore::RandomItemEnum(int x, int y)
{
	if (std::rand() % 2)
		return COAL;

	if ((x / TILE_IN_CHUNK_ROW + y / TILE_IN_CHUNK_ROW) % 6 == 1)
		return STONE;

	if (x / TILE_IN_CHUNK_ROW % 2 ^ y / TILE_IN_CHUNK_ROW % 2)
		return IRON_ORE;

	return COPPER_ORE;
}
