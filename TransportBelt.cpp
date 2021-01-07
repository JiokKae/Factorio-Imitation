#include "TransportBelt.h"
#include "Tile.h"
HRESULT TransportBelt::Init(int x, int y)
{
	itemId = ItemEnum::TRANSPORT_BELT;
	Structure::Init(x, y);

	image = new GLImage();
	image->Init(string("Entity/" + g_itemSpecs[itemId].name).c_str(), 16, 20);

	aroundBelts = new TransportBelt*[DIRECTION_END]();
	Tile* tile = TileManager::GetSingleton()->GetLpTile(coord.x, coord.y);
	for (int i = 0; i < DIRECTION_END; i++)
	{
		Tile* aroundTile = tile->GetAroundTile((DIRECTION)i);
		if (aroundTile)
		{
			Structure* structure = aroundTile->GetLpSturcture();
			if (structure && structure->GetItemId() == TRANSPORT_BELT)
			{
				aroundBelts[(DIRECTION)i] = (TransportBelt*)structure;
				((TransportBelt*)structure)->SetAroundBelts(DIRECTION((i + 2) % DIRECTION_END), this);
			}
		}
	}

	passable = true;

	return S_OK;
}

void TransportBelt::Release()
{
	SAFE_ARR_DELETE(aroundBelts);
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
	switch (direction)
	{
	case NORTH:
		if(aroundBelts[EAST] && aroundBelts[EAST]->GetDirection() == WEST)
			image->Render(lpShader, position.x, position.y,	frame % maxFrame.x, 15);
		else
			image->Render(lpShader, position.x, position.y, frame % maxFrame.x, 17);
		break;
	case EAST:
		if (aroundBelts[SOUTH] && aroundBelts[SOUTH]->GetDirection() == NORTH)
			image->Render(lpShader, position.x, position.y, frame % maxFrame.x, 11);
		else
			image->Render(lpShader, position.x, position.y,	frame % maxFrame.x, 19);
		break;
	case SOUTH:
		if (aroundBelts[WEST] && aroundBelts[WEST]->GetDirection() == EAST)
			image->Render(lpShader, position.x, position.y, frame % maxFrame.x, 8);
		else
			image->Render(lpShader, position.x, position.y,	frame % maxFrame.x, 16);
		break;
	case WEST:
		if (aroundBelts[NORTH] && aroundBelts[NORTH]->GetDirection() == SOUTH)
			image->Render(lpShader, position.x, position.y, frame % maxFrame.x, 12);
		else
			image->Render(lpShader, position.x, position.y,	frame % maxFrame.x, 18);
		break;
	}
	oframe++;
}

void TransportBelt::SetAroundBelts(DIRECTION direction, TransportBelt* aroundBelt)
{
	aroundBelts[direction] = aroundBelt;
}
