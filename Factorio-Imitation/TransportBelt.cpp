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
	speed = 45.0f;
	Structure::Init(x, y, direction, temp);
	status = WORKING;

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
			if (structure && IsTransportBelt(structure->GetItemId()))
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
	Structure::Release();

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
	int frame = g_time * speed;
	glm::ivec2 maxFrame = image->GetMaxFrame();

	image->Render(lpShader, position.x, position.y, frame % maxFrame.x, imageFrameYByDirection[direction][renderState]);
}

void TransportBelt::Render(Shader* lpShader)
{
	int frame = g_time * speed;
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
	int frame = g_time * speed;
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

void TransportBelt::FlowItem(Entity* item, bool isItem)
{
	Vec2 itemPosition = item->GetPosition();

	switch (renderState)
	{
	case TransportBelt::UP:
	{
		Vec2 flowVector;
		switch (direction)
		{
		case NORTH:
			flowVector = Vec2(0.0f, speed * 2.0f);
			if (itemPosition.x < position.x)
				itemPosition.x = position.x - 15;
			else
				itemPosition.x = position.x + 15;
			break;
		case EAST:
			flowVector = Vec2(speed * 2.0f, 0.0f);
			if (itemPosition.y < position.y)
				itemPosition.y = position.y - 15;
			else
				itemPosition.y = position.y + 15;
			break;
		case SOUTH:
			flowVector = Vec2(0.0f, -speed * 2.0f);
			if (itemPosition.x < position.x)
				itemPosition.x = position.x - 15;
			else
				itemPosition.x = position.x + 15;
			break;
		case WEST:
			flowVector = Vec2(-speed * 2.0f, 0.0f);
			if (itemPosition.y < position.y)
				itemPosition.y = position.y - 15;
			else
				itemPosition.y = position.y + 15;
			break;
		}
		if(isItem)
			item->SetPosition(itemPosition + flowVector * TimerManager::GetSingleton()->GetTimeElapsed());
		else
			item->SetPosition(item->GetPosition() + flowVector * TimerManager::GetSingleton()->GetTimeElapsed());
		break;
	}
	case TransportBelt::LEFT_UP:
	{
		Vec2 zeroPoint;
		switch (direction)
		{
		case NORTH:	zeroPoint = Vec2(position.x - TILE_SIZE / 2, position.y + TILE_SIZE / 2); break;
		case EAST:	zeroPoint = Vec2(position.x + TILE_SIZE / 2, position.y + TILE_SIZE / 2); break;
		case SOUTH:	zeroPoint = Vec2(position.x + TILE_SIZE / 2, position.y - TILE_SIZE / 2); break;
		case WEST:	zeroPoint = Vec2(position.x - TILE_SIZE / 2, position.y - TILE_SIZE / 2); break;
		}
		float angle = glm::atan((itemPosition.y - zeroPoint.y) , (itemPosition.x - zeroPoint.x)) + glm::pi<double>() / 2.025f;
		item->SetPosition(itemPosition + Vec2(cos(angle), sin(angle)) * speed * 2.0f * TimerManager::GetSingleton()->GetTimeElapsed());
		break;
	}
	
	case TransportBelt::RIGHT_UP:
	{
		Vec2 zeroPoint;
		switch (direction)
		{
		case NORTH:	zeroPoint = Vec2(position.x + TILE_SIZE / 2, position.y + TILE_SIZE / 2); break;
		case EAST:	zeroPoint = Vec2(position.x + TILE_SIZE / 2, position.y - TILE_SIZE / 2); break;
		case SOUTH:	zeroPoint = Vec2(position.x - TILE_SIZE / 2, position.y - TILE_SIZE / 2); break;
		case WEST:	zeroPoint = Vec2(position.x - TILE_SIZE / 2, position.y + TILE_SIZE / 2); break;
		}
		float angle = glm::atan((itemPosition.y - zeroPoint.y), (itemPosition.x - zeroPoint.x)) + glm::pi<double>() / 2.025f;
		item->SetPosition(itemPosition + Vec2(-cos(angle), -sin(angle)) * speed * 2.0f * TimerManager::GetSingleton()->GetTimeElapsed());
		break;
	}
	
	}
}

bool TransportBelt::InputItem(ItemInfo* inputItem, glm::vec2 pos)
{
	ItemOnGround* item = new ItemOnGround();
	item->Init((ItemEnum)inputItem->id, this->position + pos);
	EntityManager::GetSingleton()->AddItemOnGround(item);

	SAFE_DELETE(inputItem);
	return true;
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
			if (structure && IsTransportBelt(structure->GetItemId()))
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

bool TransportBelt::IsTransportBelt(int itemEnum)
{
	switch (itemEnum)
	{
	case TRANSPORT_BELT:
	case FAST_TRANSPORT_BELT:
	case EXPRESS_TRANSPORT_BELT:
		return true;
	}
	return false;
}
