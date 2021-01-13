#include "TransportBelt.h"
#include "Tile.h"
HRESULT TransportBelt::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::TRANSPORT_BELT;
	Structure::Init(x, y, direction, temp);

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
				if(!temp)
					((TransportBelt*)structure)->SetAroundBelts(DIRECTION((i + 2) % DIRECTION_END), this);
			}
		}
	}

	imageFrameYByDirection[NORTH][ImageIndex::TOP] =		2;
	imageFrameYByDirection[NORTH][ImageIndex::BOTTOM] =		7;
	imageFrameYByDirection[NORTH][ImageIndex::UP] =			17;
	imageFrameYByDirection[NORTH][ImageIndex::LEFT_UP] =	13;
	imageFrameYByDirection[NORTH][ImageIndex::RIGHT_UP] =	15;

	imageFrameYByDirection[EAST][ImageIndex::TOP] =			0;
	imageFrameYByDirection[EAST][ImageIndex::BOTTOM] =		5;
	imageFrameYByDirection[EAST][ImageIndex::UP] =			19;
	imageFrameYByDirection[EAST][ImageIndex::LEFT_UP] =		14;
	imageFrameYByDirection[EAST][ImageIndex::RIGHT_UP] =	11;

	imageFrameYByDirection[SOUTH][ImageIndex::TOP] =		6;
	imageFrameYByDirection[SOUTH][ImageIndex::BOTTOM] =		3;
	imageFrameYByDirection[SOUTH][ImageIndex::UP] =			16;
	imageFrameYByDirection[SOUTH][ImageIndex::LEFT_UP] =	10;
	imageFrameYByDirection[SOUTH][ImageIndex::RIGHT_UP] =	8;

	imageFrameYByDirection[WEST][ImageIndex::TOP] =			4;
	imageFrameYByDirection[WEST][ImageIndex::BOTTOM] =		1;
	imageFrameYByDirection[WEST][ImageIndex::UP] =			18;
	imageFrameYByDirection[WEST][ImageIndex::LEFT_UP] =		9;
	imageFrameYByDirection[WEST][ImageIndex::RIGHT_UP] =	12;

	imageTopPosOffset[NORTH] =	{ 0,  TILE_SIZE };
	imageTopPosOffset[EAST] =	{  TILE_SIZE, 0 };
	imageTopPosOffset[SOUTH] =	{ 0, -TILE_SIZE };
	imageTopPosOffset[WEST] =	{ -TILE_SIZE, 0 };

	return S_OK;
}

void TransportBelt::Release()
{
	SAFE_ARR_DELETE(aroundBelts);
	SAFE_DELETE(image);
}

void TransportBelt::Update()
{
	Structure::Update();

	// 뒤에 벨트가 있다면
	if (aroundBelts[OPPOSITE_DIR(direction)] && aroundBelts[OPPOSITE_DIR(direction)]->GetDirection() == direction)
		renderState = UP;

	// 뒤에 벨트가 없다면
	else
	{
		// 양쪽 검사
		bool left = aroundBelts[LEFT_DIR(direction)] && aroundBelts[LEFT_DIR(direction)]->GetDirection() == RIGHT_DIR(direction);
		bool right = aroundBelts[RIGHT_DIR(direction)] && aroundBelts[RIGHT_DIR(direction)]->GetDirection() == LEFT_DIR(direction);
		if (left ^ right)	// 둘중 하나만 열려 있다면
		{
			if (left)
				renderState = LEFT_UP;
			else
				renderState = RIGHT_UP;
		}
		else	// 둘다 없거나 둘다 있거나
			renderState = UP;
	}

}

void TransportBelt::FirstRender(Shader* lpShader)
{
	int frame = g_time * 30;
	glm::ivec2 maxFrame = image->GetMaxFrame();

	image->Render(lpShader, position.x, position.y, frame % maxFrame.x, imageFrameYByDirection[direction][renderState]);
}

void TransportBelt::Render(Shader* lpShader)
{
	int frame = g_time * 30;
	glm::ivec2 maxFrame = image->GetMaxFrame();

	TransportBelt* upBelt = aroundBelts[direction];
	if (upBelt)
	{
		if (!(upBelt->GetDirection() == OPPOSITE_DIR(direction)
			|| upBelt->GetDirection() == direction && upBelt->GetRenderState() == UP
			|| upBelt->GetDirection() == LEFT_DIR(direction) && upBelt->GetRenderState() == LEFT_UP
			|| upBelt->GetDirection() == RIGHT_DIR(direction) && upBelt->GetRenderState() == RIGHT_UP))
			image->Render(lpShader, position.x + imageTopPosOffset[direction].x, position.y + imageTopPosOffset[direction].y, frame % maxFrame.x, imageFrameYByDirection[direction][ImageIndex::TOP]);
	}
	else
		image->Render(lpShader, position.x + imageTopPosOffset[direction].x, position.y + imageTopPosOffset[direction].y, frame % maxFrame.x, imageFrameYByDirection[direction][ImageIndex::TOP]);

	if (renderState == UP)
	{
		TransportBelt* downBelt = aroundBelts[OPPOSITE_DIR(direction)];
		if (downBelt)
		{
			if (!(downBelt->GetDirection() == direction
				|| downBelt->GetDirection() == OPPOSITE_DIR(direction) && downBelt->GetRenderState() == UP
				|| downBelt->GetDirection() == LEFT_DIR(direction) && downBelt->GetRenderState() == RIGHT_UP
				|| downBelt->GetDirection() == RIGHT_DIR(direction) && downBelt->GetRenderState() == LEFT_UP))
				image->Render(lpShader, position.x - imageTopPosOffset[direction].x, position.y - imageTopPosOffset[direction].y, frame % maxFrame.x, imageFrameYByDirection[direction][ImageIndex::BOTTOM]);
		}
		else
			image->Render(lpShader, position.x - imageTopPosOffset[direction].x, position.y - imageTopPosOffset[direction].y, frame % maxFrame.x, imageFrameYByDirection[direction][ImageIndex::BOTTOM]);
	}

}

void TransportBelt::LateRender(Shader* lpShader)
{

}

void TransportBelt::SetPosition(Vec2 position)
{
	Entity::SetPosition(position);
	coord = glm::ivec2(POS_TO_COORD(this->position));

	Tile* tile = TileManager::GetSingleton()->GetLpTile(coord.x, coord.y);
	for (int i = 0; i < DIRECTION_END; i++)
	{
		aroundBelts[(DIRECTION)i] = nullptr;
		Tile* aroundTile = tile->GetAroundTile((DIRECTION)i);
		if (aroundTile)
		{
			Structure* structure = aroundTile->GetLpSturcture();
			if (structure && structure->GetItemId() == TRANSPORT_BELT)
			{
				aroundBelts[(DIRECTION)i] = (TransportBelt*)structure;
				//((TransportBelt*)structure)->SetAroundBelts(DIRECTION((i + 2) % DIRECTION_END), this);
			}
		}
	}
}

void TransportBelt::SetAroundBelts(DIRECTION direction, TransportBelt* aroundBelt)
{
	aroundBelts[direction] = aroundBelt;
}
