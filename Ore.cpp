#include "Ore.h"
#include "GLImage.h"
#include "Shader.h"

HRESULT Ore::Init()
{
	amount = rand() % 12000;
	if (amount > 8000)
		amount = 0;
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