#include "AssemblingMachine1.h"
#include "GLImage.h"

HRESULT AssemblingMachine1::Init(int x, int y, DIRECTION direction)
{
	itemId = ItemEnum::ASSEMBLING_MACHINE_1;
	Structure::Init(x, y, direction);

	image = new GLImage();
	image->Init(string("Entity/" + g_itemSpecs[itemId].name).c_str(), 8, 4);

	shadow = new GLImage();
	shadow[DIRECTION::NORTH].Init(string("Entity/" + g_itemSpecs[itemId].name + "-shadow").c_str(), 8, 4);
	shadow[DIRECTION::NORTH].SetAlpha(0.6f);
	shadowAniOffset[DIRECTION::NORTH] = { 50, 0 };

	return S_OK;
}

void AssemblingMachine1::Release()
{
	for (int i = 0; i < DIRECTION_END; i++)
		image->Release();
	SAFE_ARR_DELETE(image);

	for (int i = 0; i < DIRECTION_END; i++)
		shadow->Release();
	SAFE_ARR_DELETE(shadow);
}

void AssemblingMachine1::Update()
{
	Structure::Update();

	if (PtInFRect(GetFRect(), g_cursorPosition))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			UIManager::GetSingleton()->ActiveUI("BurnerMiningDrillUI");
		}
	}
}

void AssemblingMachine1::Render(Shader* lpShader)
{
	static int am1frame;
	int frame = am1frame / 100;
	glm::ivec2 maxFrame = image->GetMaxFrame();
	shadow[direction].Render(lpShader, position.x + shadowAniOffset[direction].x, position.y + shadowAniOffset[direction].y,
		maxFrame.x - 1 - frame % maxFrame.x, frame / maxFrame.x % maxFrame.y);
	image[direction].Render(lpShader, position.x, position.y, 
		maxFrame.x - 1 - frame % maxFrame.x, frame / maxFrame.x % maxFrame.y);
	am1frame++;
}
