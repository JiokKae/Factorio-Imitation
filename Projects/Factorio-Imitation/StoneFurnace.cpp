#include "StoneFurnace.h"
#include "Tile.h"
#include "Ore.h"
#include "ItemOnGround.h"
#include "StoneFurnaceUI.h"
#include "Recipe.h"

HRESULT StoneFurnace::Init(int x, int y, DIRECTION _direction, bool _temp)
{
	usingClickEvent = true;
	itemId = ItemEnum::STONE_FURNACE;
	Structure::Init(x, y, _direction, _temp);

	// 이미지 초기화
	mainImage = new GLImage();
	mainImage->Init("Entity/StoneFurnace");
	shadowImage = new GLImage();
	shadowImage->Init("Entity/StoneFurnace-shadow");
	shadowImage->SetAlpha(0.6f);
	fireImage = new GLImage();
	fireImage->Init("Entity/StoneFurnace-fire", 8, 6);

	// 초기값 세팅
	energyConsumption = 90.0f;
	craftingSpeed = 1.0f;

	// 레시피 세팅
	vecRecipes = RecipeManager::GetSingleton()->FindRecipes(STONE_FURNACE);

	// 아이템 슬롯 세팅
	fuel = new ItemInfo();
	resource = new ItemInfo();
	result = new ItemInfo();
	for (int i = 0; i < vecRecipes->size(); i++)
	{
		resource->AddAbleItem((*vecRecipes)[i]->GetIngredient(0).id);
		result->AddAbleItem((*vecRecipes)[i]->GetOutput().id);
	}
	usingResource = new ItemInfo();
	newResult = new ItemInfo();

	return S_OK;
}

void StoneFurnace::Release()
{
	Structure::Release();

	SAFE_RELEASE(mainImage);
	SAFE_RELEASE(shadowImage);
	SAFE_RELEASE(fireImage);

	SAFE_DELETE(fuel);
	SAFE_DELETE(resource);
	SAFE_DELETE(usingResource);
	SAFE_DELETE(result);
	SAFE_DELETE(newResult);
}

void StoneFurnace::Update()
{
	Structure::Update();

	if (temp)
		return;

	switch (status)
	{
	case StoneFurnace::NO_POWER:
		if (fuel->amount)
		{
			if (resource->amount)
				status = WORKING;
			else
				status = NO_RECIPE;
		}
		break;

	case StoneFurnace::NO_RECIPE:
		if (currPower <= 0 && fuel->amount < 0)
		{
			status = NO_POWER;
		}
		else if (resource->amount && resource->amount >= FindRecipeByIngredient(resource->id)->GetIngredient(0).amount)
		{
			status = WORKING;
		}
		break;

	case StoneFurnace::WORKING:
		// 에너지가 없고 연료가 없다면
		if (currPower <= 0 && fuel->amount <= 0)
		{
			status = NO_POWER;
			break;
		}
		// 재료가 없고 사용중인 재료가 없다면
		else if (resource->amount < FindRecipeByIngredient(resource->id)->GetIngredient(0).amount && usingResource->amount <= 0)
		{
			status = NO_RECIPE;
			break;
		}
		// 생산
		else
		{
			// 제작 완료한 완성품이 남아있다면
			if (newResult->amount)
			{
				status = ITEM_PRODUCTION_OVERLOAD;
				break;
			}

			// 에너지가 없다면 연료를 사용
			if (currPower <= 0)
			{
				fuel->amount--;
				currPower += g_itemSpecs[fuel->id].fuelValue;
				maxPower = g_itemSpecs[fuel->id].fuelValue;
			}

			// 사용중인 재료 없을 때 재료 투입
			if (usingResource->amount <= 0)
			{
				currRecipe = FindRecipeByIngredient(resource->id);
				resource->amount -= currRecipe->GetIngredient(0).amount;
				usingResource->amount += currRecipe->GetIngredient(0).amount;
			}

			// 에너지 소모
			currPower -= energyConsumption * TimerManager::GetSingleton()->GetTimeElapsed();
			craftedTime += craftingSpeed * TimerManager::GetSingleton()->GetTimeElapsed();

			// 제작 완료
			if (craftedTime >= currRecipe->GetCraftingTime())
			{
				usingResource->amount -= currRecipe->GetIngredient(0).amount;
				newResult->id = currRecipe->GetOutput().id;
				newResult->amount = currRecipe->GetOutput().amount;
				productsFinished++;
				craftedTime -= currRecipe->GetCraftingTime();

				status = ITEM_PRODUCTION_OVERLOAD;
			}

			productionPercent = craftedTime / currRecipe->GetCraftingTime();
			time += TimerManager::GetSingleton()->GetTimeElapsed();
		}
		break;

	case StoneFurnace::ITEM_PRODUCTION_OVERLOAD:
		// 기존의 완성품이 있다면
		if (result->amount)
		{
			// 새로 만들어진 완성품과 기존에 있던 완성품이 같다면
			if (newResult->id == result->id)
			{
				result->amount += newResult->amount;
				newResult->amount = 0;
				status = WORKING;
			}
		}
		// 기존의 완성품이 없다면
		else
		{
			result->id = newResult->id;
			result->amount = newResult->amount;
			newResult->amount = 0;
			status = WORKING;
		}
		break;

	case StoneFurnace::DESTORY:
		break;

	}

}

void StoneFurnace::FirstRender(Shader* shader)
{
	shadowImage->Render(shader, position.x + 27, position.y - 14);
}

void StoneFurnace::Render(Shader* shader)
{
	mainImage->Render(shader, position.x, position.y);

	if (status == WORKING)
	{
		glm::ivec2 maxFrame = fireImage->GetMaxFrame();
		int frame = (int)(time * 30);

		int frameX = frame % maxFrame.x;
		int frameY = maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y;
		fireImage->Render(shader, position.x, position.y, frameX, frameY);
	}
}

void StoneFurnace::RenderInScreen(Shader* shader, float posX, float posY)
{
	mainImage->Render(shader, posX, posY);

	if (status == WORKING)
	{
		glm::ivec2 maxFrame = fireImage->GetMaxFrame();
		int frame = (int)(time * 30);

		int frameX = frame % maxFrame.x;
		int frameY = maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y;
		fireImage->Render(shader, posX, posY, frameX, frameY);
	}
}

bool StoneFurnace::InputItem(ItemInfo* inputItem, glm::vec2 /*pos*/)
{
	// 받는 아이템이 연료라면
	if (g_itemSpecs[inputItem->id].fuel)
	{
		// 연료 슬롯에 연료가 있다면
		if (fuel->amount)
		{
			// 연료 종류가 같을 때만
			if (fuel->id == inputItem->id)
			{
				fuel->amount += inputItem->amount;

				SAFE_DELETE(inputItem);
				return true;
			}
		}
		// 연료 슬롯에 연료가 없다면
		else
		{
			fuel->id = inputItem->id;
			fuel->amount = inputItem->amount;

			SAFE_DELETE(inputItem);
			return true;
		}
	}
	else if (FindRecipeByIngredient(inputItem->id))
	{
		// 재료 슬롯에 재료가 있다면
		if (resource->amount)
		{
			// 재료 종류가 같을 때만
			if (resource->id == inputItem->id)
			{
				resource->amount += inputItem->amount;

				SAFE_DELETE(inputItem);
				return true;
			}
		}
		// 재료 슬롯이 비어 있다면
		else
		{
			resource->id = inputItem->id;
			resource->amount = inputItem->amount;

			SAFE_DELETE(inputItem);
			return true;
		}
		
	}

	SAFE_DELETE(inputItem);
	return false;
}

bool StoneFurnace::TakeOutItem(ItemInfo* outItem)
{
	// 내보낼 생산품이 있다면
	if (result->amount)
	{
		// 생산품의 개수가 반출 요구치를 넘으면
		if (result->amount >= outItem->amount)
		{
			result->amount -= outItem->amount;
			outItem->id = result->id;
		}
		// 생산품의 개수가 반출 요구치를 넘지 못하면
		else
		{
			outItem->id = result->id;
			outItem->amount = result->amount;
			result->amount = 0;
		}
		return true;
	}
	// 내보낼 생산품이 없다면
	else
		return false;
}

void StoneFurnace::ClickEvent()
{
	UIManager::GetSingleton()->ActiveUI("StoneFurnaceUI");
	StoneFurnaceUI* sfUI = (StoneFurnaceUI*)UIManager::GetSingleton()->GetLpCurrUI();
	sfUI->SetCurrStoneFurnace(this);
}

Recipe* StoneFurnace::FindRecipeByIngredient(int itemEnum)
{
	for (int i = 0; i < vecRecipes->size(); i++)
	{
		if ((*vecRecipes)[i]->IsIngredient(itemEnum))
			return (*vecRecipes)[i];
	}
	return nullptr;
}

string StoneFurnace::ToString()
{
	char buf[128];
	wsprintf(buf, " Coord: (%d, %d)\n Status: %s", coord.x, coord.y, statusString[status]);
	return string(buf);
};
