#include "AssemblingMachine1.h"
#include "GLImage.h"

HRESULT AssemblingMachine1::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::ASSEMBLING_MACHINE_1;
	Structure::Init(x, y, direction, temp);

	image = new GLImage();
	image->Init("Entity/AssemblingMachine1", 8, 4);
	image->SetAlpha(1.0f);

	shadow = new GLImage();
	shadow->Init("Entity/AssemblingMachine1-shadow", 8, 4);
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
			//UIManager::GetSingleton()->ActiveUI("BurnerMiningDrillUI");
		}
	}
}

void AssemblingMachine1::FirstRender(Shader* shader)
{
	int frame = g_time * 30;
	glm::ivec2 maxFrame = image->GetMaxFrame();
	shadow->Render(shader, position.x + shadowAniOffset.x, position.y + shadowAniOffset.y,
		frame % maxFrame.x, maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y);
}

void AssemblingMachine1::Render(Shader* shader)
{
	Render(shader, position.x, position.y);
}

void AssemblingMachine1::Render(Shader* shader, float posX, float posY)
{
	int frame = g_time * 30;
	glm::ivec2 maxFrame = image->GetMaxFrame();
	image->Render(shader, posX, posY,
		frame % maxFrame.x, maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y);
}
