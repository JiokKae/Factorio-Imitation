#include "Ore.h"
#include "GLImage.h"
#include "Shader.h"
#include "ItemEnum.h"

HRESULT Ore::Init(int x, int y)
{
	if (rand() % 2)
	{
		if ((x / CHUNK_IN_TILE + y / CHUNK_IN_TILE) % 6 == 1)
			itemEnum = STONE;
		else if (x / CHUNK_IN_TILE % 2 ^ y / CHUNK_IN_TILE % 2)
			itemEnum = IRON_ORE;
		else
			itemEnum = COPPER_ORE;
	}
	else
		itemEnum = COAL;

	amount = int(10.0f - glm::distance(glm::vec2(16, 16), glm::vec2(abs(x%32), abs(y%32)))) * (300 + rand() % 200);
	if (amount < 0)
		amount = 0;
//	amount = rand() % 12000;
	//if (amount > 2000)
//		amount = 0;


	randFrameX = rand() % 8;
	return S_OK;
}

void Ore::Release()
{
}

void Ore::Update()
{
}

int Ore::AmountToImageFrameY()
{
	if (amount > 10000)
		return 7;
	else if (amount > 8000)
		return 6;
	else if (amount > 6000)
		return 5;
	else if (amount > 4000)
		return 4;
	else if (amount > 1500)
		return 3;
	else if (amount > 300)
		return 2;
	else if (amount > 200)
		return 1;
	else
		return 0;
}