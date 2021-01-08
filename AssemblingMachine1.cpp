#include "AssemblingMachine1.h"
#include "GLImage.h"

HRESULT AssemblingMachine1::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::ASSEMBLING_MACHINE_1;
	Structure::Init(x, y, direction, temp);

	image = new GLImage();
	image->Init(string("Entity/" + g_itemSpecs[itemId].name).c_str(), 8, 4);
	image->SetAlpha(1.0f);

	shadow = new GLImage();
	shadow->Init(string("Entity/" + g_itemSpecs[itemId].name + "-shadow").c_str(), 8, 4);
	shadow->SetAlpha(0.5f);
	shadowAniOffset = { 16, -6 };

	return S_OK;
}

void AssemblingMachine1::Release()
{
	SAFE_RELEASE(image);
	SAFE_RELEASE(shadow);
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
	int frame = g_time * 30;
	glm::ivec2 maxFrame = image->GetMaxFrame();
	shadow->Render(lpShader, position.x + shadowAniOffset.x, position.y + shadowAniOffset.y,
		frame % maxFrame.x, maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y);
	image->Render(lpShader, position.x, position.y, 
		frame % maxFrame.x, maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y);
}
