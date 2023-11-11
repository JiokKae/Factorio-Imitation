#include "BurnerInserter.h"
#include "Tile.h"
#include "ItemOnGround.h"

HRESULT BurnerInserter::Init(int x, int y, DIRECTION _direction, bool _temp)
{
	itemId = ItemId::BURNER_INSERTER;
	Structure::Init(x, y, _direction, _temp);
	status = WORKING;

	platformImage = new GLImage("Entity/BurnerInserter-platform", 4, 1);

	handBaseImage = new GLImage("Entity/BurnerInserter-handBase");
	handBaseScale = { 0.4f, 0.4f };
	handBaseImage->SetScale(handBaseScale);
	handBaseImage->SetOffset(glm::vec2(0.0f, 60.0f));
	handBaseAngle = 45;

	handOpenImage = new GLImage("Entity/BurnerInserter-handOpen");
	handOpenScale = { 0.4f, 0.4f };
	handOpenImage->SetScale(handOpenScale);
	handOpenImage->SetOffset({ 0, 82 });
	handOpenAngle = -45;

	allItemsImage = new GLImage("Icons/AllItems", 8, 8, 0.25f, 0.25f);
	allItemsImage->SetScale({0.25f, 0.25f});
	
	handAngle = -90.0f * static_cast<int>(_direction);
	handAngleSpeed = 216.0f;


	return S_OK;
}

void BurnerInserter::Release()
{
	Structure::Release();

	SAFE_DELETE(handOpenImage);
	SAFE_DELETE(handBaseImage);
	SAFE_DELETE(platformImage);
	SAFE_DELETE(allItemsImage);
}

void BurnerInserter::Update()
{
	Structure::Update();
	switch (status)
	{
	case Structure::NO_POWER:
		break;
	case Structure::WORKING:
	{
		float destAngle;
		if (handItem.amount == 0)
			destAngle = destAngles[direction];
		else
			destAngle = destAngles[OPPOSITE_DIR(direction)];

		float deltaAngle = handAngle - destAngle;
		if (glm::abs(deltaAngle) <= handAngleSpeed * TimerManager::GetSingleton()->GetTimeElapsed())
		{
			handAngle = destAngle;
			lpDirectionTile = TileManager::GetSingleton()->GetLpTile(position.x, position.y)->GetAroundTile(direction);
			if(handItem.amount == 0)
				status = WAITING_SOURCE_ITEMS;
			else
				status = WAITING_SPACE;
		}
		else if (glm::abs(deltaAngle) > 181.0f && glm::abs(deltaAngle) < 361.0f)
		{
			if (handAngle < -269.0)
				handAngle += 360;
			else
				handAngle -= 360;
		}

		else if (deltaAngle > 0)
			handAngle -= handAngleSpeed * TimerManager::GetSingleton()->GetTimeElapsed();
		else
			handAngle += handAngleSpeed * TimerManager::GetSingleton()->GetTimeElapsed();

		// handbase 업데이트
		handBaseRenderAngle = handAngle - sin(glm::radians(handAngle)) * handBaseAngle;
		handBaseImage->SetAngle(glm::radians(handBaseRenderAngle));
		
		float scaleY = ((cos(glm::radians(handBaseRenderAngle)) + 1.0f) / 4.0f + 0.4f);
		handBaseScale.y = 0.4f * scaleY;
		handBaseImage->SetScale(handBaseScale);

		// handOpen 업데이트
		handOpenRenderAngle = handAngle - sin(glm::radians(handAngle)) * handOpenAngle;
		handOpenImage->SetAngle(glm::radians(handOpenRenderAngle));
		if(handOpenAngle > 0)
			handOpenScale.y = ((cos(glm::radians(handOpenRenderAngle)) + 1.0f) / 4.0f + 0.4f) * 0.4f;
		else
			handOpenScale.y = ((-cos(glm::radians(handOpenRenderAngle)) + 1.0f) / 4.0f + 0.4f) * 0.4f;
		handOpenImage->SetScale(handOpenScale);

		break;

	}
	case Structure::WAITING_SPACE:
		if (glm::abs(handAngle - destAngles[OPPOSITE_DIR(direction)]) > glm::epsilon<float>())
			status = WORKING;

		lpDestTile = TileManager::GetSingleton()->GetLpTile(position.x, position.y)->GetAroundTile((DIRECTION)OPPOSITE_DIR(direction));
		if (lpDestTile->GetLpSturcture())
		{
			if (lpDestTile->GetLpSturcture()->InputItem(&handItem, glm::vec2(16, 16)))
			{
				status = WORKING;
			}
		}
		break;

	case Structure::WAITING_SOURCE_ITEMS:
	{
		if (glm::abs(handAngle - destAngles[direction]) > glm::epsilon<float>())
			status = WORKING;

		if (temp)
			return;

		Structure* lpDirectionStructure = lpDirectionTile->GetLpSturcture();
		if (lpDirectionStructure)
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

		if(lpDirectionTile->GetItems().size())
		{
			ItemOnGround* item = lpDirectionTile->GetItems().at(0);
			handItem.id = item->GetItemId();
			handItem.amount = 1;
			EntityManager::GetSingleton()->DeleteItemOnGround(item);
			status = WORKING;
		}
		break;
	}
	
	case Structure::NO_MINABLE_RESOURCES:
		break;

	case Structure::DESTORY:
		break;

	case Structure::ITEM_PRODUCTION_OVERLOAD:
		break;

	}


}

void BurnerInserter::FirstRender(ShaderProgram* /*lpShaderProgram*/)
{
}

void BurnerInserter::Render(ShaderProgram* lpShader)
{
	platformImage->Render(lpShader, position.x, position.y, OPPOSITE_DIR(direction), 0);
	
}

void BurnerInserter::LateRender(ShaderProgram* lpShader)
{
	handOpenImage->Render(lpShader, 
		position.x - sin(glm::radians(handBaseRenderAngle)) * 120 * handBaseScale.y ,
		position.y + cos(glm::radians(handBaseRenderAngle)) * 120 * handBaseScale.y + 10);
	
	handBaseImage->Render(lpShader, position.x, position.y + 10);

	if (handItem.amount != 0)
	{
		allItemsImage->Render(lpShader,
			position.x - sin(glm::radians(handBaseRenderAngle)) * 120 * handBaseScale.y - sin(glm::radians(handOpenRenderAngle)) * 136 * handOpenScale.y,
			position.y + cos(glm::radians(handBaseRenderAngle)) * 120 * handBaseScale.y + cos(glm::radians(handOpenRenderAngle)) * 136 * handOpenScale.y + 10,
			handItem.id % 8, 7 - handItem.id / 8);
	}
}

void BurnerInserter::RenderInScreen(ShaderProgram* lpShader, float posX, float posY)
{
	platformImage->Render(lpShader, posX, posY, OPPOSITE_DIR(direction), 0);

	handOpenImage->Render(lpShader,
		posX - sin(glm::radians(handBaseRenderAngle)) * 120 * handBaseScale.y,
		posY + cos(glm::radians(handBaseRenderAngle)) * 120 * handBaseScale.y + 10);

	handBaseImage->Render(lpShader, posX, posY + 10);

	if (handItem.amount != 0)
	{
		allItemsImage->Render(lpShader,
			posX - sin(glm::radians(handBaseRenderAngle)) * 120 * handBaseScale.y - sin(glm::radians(handOpenRenderAngle)) * 136 * handOpenScale.y,
			posY + cos(glm::radians(handBaseRenderAngle)) * 120 * handBaseScale.y + cos(glm::radians(handOpenRenderAngle)) * 136 * handOpenScale.y + 10,
			handItem.id % 8, 7 - handItem.id / 8);
	}
}

string BurnerInserter::ToString()
{
	char buf[128]; 
	sprintf_s(buf, " Coord: (%d, %d)\n Status: %s\n HandAngle: %.1f", coord.x, coord.y, statusString[status], handAngle);
	return string(buf);
}

BurnerInserter::BurnerInserter()
{
	allItemsImage = nullptr;
	platformImage = nullptr;
	handOpenImage = nullptr;
	handBaseImage = nullptr;
	
	lpDestTile = nullptr;
	lpDirectionTile = nullptr;

	handAngle = 0.0f;
	handAngleSpeed = 0.0f;
	handOpenAngle = 0.0f;
	handOpenRenderAngle = 0.0f;
	handBaseAngle = 0.0f;
	handBaseRenderAngle = 0.0f;
}

