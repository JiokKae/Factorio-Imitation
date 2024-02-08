#include "BurnerInserter.h"
#include "Tile.h"
#include "ItemOnGround.h"

float Degree::Sin(float degree)
{
	return sin(Rad(degree));
}

float Degree::Cos(float degree)
{
	return cos(Rad(degree));
}

float Degree::Rad(float degree)
{
	return glm::radians(degree);
}

BurnerInserter::HandPart::HandPart(const std::string& textureKey, const glm::vec2& scale, const glm::vec2& offset, float degree, float innerLength)
	: image{ std::make_unique<GLImage>(textureKey) }
	, degree{ degree }
	, renderDegree{ 0.0f }
	, innerLength{ innerLength }
	, baseScaleY{ scale.y }
{
	image->SetScale(scale);
	image->SetOffset(offset);
}

void BurnerInserter::HandPart::Update(float handDegree)
{
	renderDegree = handDegree - Degree::Sin(handDegree) * degree;

	image->SetAngle(Degree::Rad(renderDegree));

	const float sign = (degree > 0) ? 1.0f : -1.0f;
	const float scaleY = (sign * Degree::Cos(renderDegree) * 0.25f + 0.65f) * baseScaleY;

	image->SetScale({ image->GetScale().x, scaleY });
}

void BurnerInserter::HandPart::Render(ShaderProgram* shader, float posX, float posY)
{
	image->Render(shader, posX, posY);
}

float BurnerInserter::HandPart::EndpointPosX() const
{
	return -Degree::Sin(renderDegree) * innerLength * image->GetScale().y;
}

float BurnerInserter::HandPart::EndpointPosY() const
{
	return Degree::Cos(renderDegree) * innerLength * image->GetScale().y;
}

BurnerInserter::BurnerInserter()
	: handBase("Entity/BurnerInserter-handBase", { 0.4f, 0.4f }, { 0.0f, 60.0f }, 45.0f, 120.0f)
	, handOpen("Entity/BurnerInserter-handOpen", { 0.4f, 0.4f }, { 0.0f, 82.0f }, -45.0f, 136.0f)
	, platformImage{ std::make_unique<GLImage>("Entity/BurnerInserter-platform", 4, 1) }
	, allItemsImage{ std::make_unique<GLImage>("Icons/AllItems", 8, 8, 0.25f, 0.25f) }
	, handDegreeSpeed{ 216.0f }
{
	itemId = ItemId::BURNER_INSERTER;

	allItemsImage->SetScale({ 0.25f, 0.25f });
}

HRESULT BurnerInserter::Init(int x, int y, DIRECTION _direction, bool _temp)
{
	Structure::Init(x, y, _direction, _temp);
	status = WORKING;

	handDegree = -90.0f * static_cast<int>(_direction);

	return S_OK;
}

void BurnerInserter::Update()
{
	Structure::Update();

	switch (status)
	{
	case Structure::WORKING:
	{
		float destDegree;
		if (handItem.amount == 0)
			destDegree = DESTINATION_ANGLES[direction];
		else
			destDegree = DESTINATION_ANGLES[OPPOSITE_DIR(direction)];

		const float remainedDegree = handDegree - destDegree;
		const float deltaDegree = handDegreeSpeed * TimerManager::GetSingleton()->GetTimeElapsed();

		if (glm::abs(remainedDegree) <= deltaDegree)
		{
			handDegree = destDegree;

			lpDirectionTile = TileManager::GetSingleton()->GetLpTile(position.x, position.y)->GetAroundTile(direction);

			if (handItem.IsEmpty() == true)
				status = WAITING_SOURCE_ITEMS;
			else
				status = WAITING_SPACE;
		}
		else if (glm::abs(remainedDegree) > 181.0f && glm::abs(remainedDegree) < 361.0f)
		{
			if (handDegree < -269.0)
				handDegree += 360.0f;
			else
				handDegree -= 360.0f;
		}
		else if (remainedDegree > 0)
			handDegree -= deltaDegree;
		else
			handDegree += deltaDegree;

		handBase.Update(handDegree);

		handOpen.Update(handDegree);

		break;

	}
	case Structure::WAITING_SPACE:
	{
		if (glm::abs(handDegree - DESTINATION_ANGLES[OPPOSITE_DIR(direction)]) > glm::epsilon<float>())
			status = WORKING;

		Tile* lpDestTile = TileManager::GetSingleton()->GetLpTile(position.x, position.y)->GetAroundTile((DIRECTION)OPPOSITE_DIR(direction));
		if (Structure* structure = lpDestTile->GetLpSturcture())
		{
			if (structure->InputItem(&handItem, glm::vec2(16, 16)))
			{
				status = WORKING;
			}
		}
		break;
	}
	case Structure::WAITING_SOURCE_ITEMS:
	{
		if (glm::abs(handDegree - DESTINATION_ANGLES[direction]) > glm::epsilon<float>())
			status = WORKING;

		if (temp)
			return;

		if (Structure* lpDirectionStructure = lpDirectionTile->GetLpSturcture())
		{
			ItemInfo getItem;
			getItem.amount = 1;
			if (lpDirectionStructure->TakeOutItem(&getItem))
			{
				handItem.id = getItem.id;
				handItem.amount = getItem.amount;
				status = WORKING;
			}
		}
		if (lpDirectionTile->GetItems().size())
		{
			ItemOnGround* item = lpDirectionTile->GetItems().at(0);
			handItem.id = item->GetItemId();
			handItem.amount = 1;
			EntityManager::GetSingleton()->DeleteItemOnGround(item);
			status = WORKING;
		}
		break;
	}
	}
}

void BurnerInserter::Render(ShaderProgram* lpShader)
{
	platformImage->Render(lpShader, position.x, position.y, OPPOSITE_DIR(direction), 0);
}

void BurnerInserter::LateRender(ShaderProgram* lpShader)
{
	handOpen.Render(lpShader, position.x + handBase.EndpointPosX(), position.y + 10 + handBase.EndpointPosY());
	
	handBase.Render(lpShader, position.x, position.y + 10);

	if (handItem.IsEmpty() == false)
	{
		allItemsImage->Render(lpShader,
			position.x + handBase.EndpointPosX() + handOpen.EndpointPosX(),
			position.y + 10 + handBase.EndpointPosY() + handOpen.EndpointPosY(),
			handItem.id % 8, 7 - handItem.id / 8);
	}
}

void BurnerInserter::RenderInScreen(ShaderProgram* lpShader, float posX, float posY)
{
	platformImage->Render(lpShader, posX, posY, OPPOSITE_DIR(direction), 0);
	
	handOpen.Render(lpShader, posX + handBase.EndpointPosX(), posY + 10 + handBase.EndpointPosY());

	handBase.Render(lpShader, posX, posY + 10);

	if (handItem.IsEmpty() == false)
	{
		allItemsImage->Render(lpShader,
			posX + handBase.EndpointPosX() + handOpen.EndpointPosX(),
			posY + 10 + handBase.EndpointPosY() + handOpen.EndpointPosY(),
			handItem.id % 8, 7 - handItem.id / 8);
	}
}

std::string BurnerInserter::ToString()
{
	char buf[128]; 
	sprintf_s(buf, " Coord: (%d, %d)\n Status: %s\n HandAngle: %.1f", coord.x, coord.y, statusString[status], handDegree);
	return std::string(buf);
}
