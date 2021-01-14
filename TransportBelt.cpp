#include "TransportBelt.h"
#include "Tile.h"
#include "ItemOnGround.h"
#include "ItemOnGrounds.h"
int TransportBelt::imageFrameYByDirection[DIRECTION_END][TransportBelt::ImageIndex::END] = {
//			TOP			BOTTOM		UP			LEFT_UP		RIGHT_UP
			2,			7,			17,			13,			15,			// NORTH
			0,			5,			19,			14,			11,			// EAST
			6,			3,			16,			10,			8,			// SOUTH
			4,			1,			18,			9,			12,			// WEST
};

glm::ivec2 TransportBelt::imageTopPosOffset[DIRECTION_END] = {
	{ 0,  TILE_SIZE },	// NORTH
	{  TILE_SIZE, 0 },	// EAST
	{ 0, -TILE_SIZE },	// SOUTH
	{ -TILE_SIZE, 0 },	// WEST
};

HRESULT TransportBelt::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::TRANSPORT_BELT;
	Structure::Init(x, y, direction, temp);

	image = new GLImage();
	image->Init("Entity/TransportBelt", 16, 20);

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

void TransportBelt::Render(Shader* shader, float posX, float posY)
{
	int frame = g_time * 30;
	glm::ivec2 maxFrame = image->GetMaxFrame();

	image->Render(shader, posX, posY, frame % maxFrame.x, imageFrameYByDirection[direction][renderState]);

	TransportBelt* upBelt = aroundBelts[direction];
	if (upBelt)
	{
		if (!(upBelt->GetDirection() == OPPOSITE_DIR(direction)
			|| upBelt->GetDirection() == direction && upBelt->GetRenderState() == UP
			|| upBelt->GetDirection() == LEFT_DIR(direction) && upBelt->GetRenderState() == LEFT_UP
			|| upBelt->GetDirection() == RIGHT_DIR(direction) && upBelt->GetRenderState() == RIGHT_UP))
			image->Render(shader, posX + imageTopPosOffset[direction].x, posY + imageTopPosOffset[direction].y, frame % maxFrame.x, imageFrameYByDirection[direction][ImageIndex::TOP]);
	}
	else
		image->Render(shader, posX + imageTopPosOffset[direction].x, posY + imageTopPosOffset[direction].y, frame % maxFrame.x, imageFrameYByDirection[direction][ImageIndex::TOP]);

	if (renderState == UP)
	{
		TransportBelt* downBelt = aroundBelts[OPPOSITE_DIR(direction)];
		if (downBelt)
		{
			if (!(downBelt->GetDirection() == direction
				|| downBelt->GetDirection() == OPPOSITE_DIR(direction) && downBelt->GetRenderState() == UP
				|| downBelt->GetDirection() == LEFT_DIR(direction) && downBelt->GetRenderState() == RIGHT_UP
				|| downBelt->GetDirection() == RIGHT_DIR(direction) && downBelt->GetRenderState() == LEFT_UP))
				image->Render(shader, posX - imageTopPosOffset[direction].x, posY - imageTopPosOffset[direction].y, frame % maxFrame.x, imageFrameYByDirection[direction][ImageIndex::BOTTOM]);
		}
		else
			image->Render(shader, posX - imageTopPosOffset[direction].x, posY - imageTopPosOffset[direction].y, frame % maxFrame.x, imageFrameYByDirection[direction][ImageIndex::BOTTOM]);
	}

}

void TransportBelt::FlowItem(ItemOnGround* item)
{
	item->SetPosition(item->GetPosition() + Vec2(0.0, 7.5 * 14 * TimerManager::GetSingleton()->GetTimeElapsed()));
}

bool TransportBelt::InputItem(ItemInfo* inputItem, glm::vec2 pos)
{
	if (pos.x >= 32)
	{
		ItemOnGround* item = new ItemOnGround();
		item->Init((ItemEnum)inputItem->id);
		item->SetPosition(this->position + pos);
		leftline.push_back(item);
		EntityManager::GetSingleton()->AddItemOnGround(item);

		SAFE_DELETE(inputItem);
		return true;
	}
	else
	{
		ItemOnGround* item = new ItemOnGround();
		item->Init((ItemEnum)inputItem->id);
		item->SetPosition(this->position + pos);
		rightline.push_back(item);
		EntityManager::GetSingleton()->AddItemOnGround(item);

		SAFE_DELETE(inputItem);
		return true;
	}
	SAFE_DELETE(inputItem);
	return false;
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
