#include "TransportBelt.h"

HRESULT TransportBelt::Init(int x, int y)
{
	itemId = ItemEnum::TRANSPORT_BELT;
	Structure::Init(x, y);

	image = new GLImage();
	image->Init(string("Entity/" + g_itemSpecs[itemId].name).c_str(), 16, 20);

	passable = true;

	return S_OK;
}

void TransportBelt::Release()
{
	SAFE_DELETE(image);
}

void TransportBelt::Update()
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

void TransportBelt::Render(Shader* lpShader)
{
	static int oframe;
	int frame = oframe / 600;
	glm::ivec2 maxFrame = image->GetMaxFrame();
	image[direction].Render(lpShader, position.x, position.y,
		maxFrame.x - 1 - frame % maxFrame.x, frame / maxFrame.x % maxFrame.y);
	oframe++;
}
