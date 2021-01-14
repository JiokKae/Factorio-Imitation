#include "StoneFurnace.h"
#include "Tile.h"
#include "Ore.h"
#include "ItemOnGround.h"
#include "StoneFurnaceUI.h"
#include "Recipe.h"

HRESULT StoneFurnace::Init(int x, int y, DIRECTION direction, bool temp)
{
	usingClickEvent = true;
	itemId = ItemEnum::STONE_FURNACE;
	Structure::Init(x, y, direction, temp);

	// �̹��� �ʱ�ȭ
	mainImage = new GLImage();
	mainImage->Init("Entity/StoneFurnace");
	shadowImage = new GLImage();
	shadowImage->Init("Entity/StoneFurnace-shadow");
	shadowImage->SetAlpha(0.6f);
	fireImage = new GLImage();
	fireImage->Init("Entity/StoneFurnace-fire", 8, 6);

	// �ʱⰪ ����
	energyConsumption = 90.0f;
	craftingSpeed = 1.0f;

	// ������ ����
	vecRecipes.push_back(RecipeManager::GetSingleton()->FindRecipe(IRON_PLATE));
	vecRecipes.push_back(RecipeManager::GetSingleton()->FindRecipe(COPPER_PLATE));
	vecRecipes.push_back(RecipeManager::GetSingleton()->FindRecipe(STONE_BRICK));
	vecRecipes.push_back(RecipeManager::GetSingleton()->FindRecipe(STEEL_PLATE));

	// ������ ���� ����
	fuel = new ItemInfo();
	resource = new ItemInfo();
	result = new ItemInfo();
	for (int i = 0; i < vecRecipes.size(); i++)
	{
		resource->AddAbleItem(vecRecipes[i]->GetIngredient(0).id);
		result->AddAbleItem(vecRecipes[i]->GetOutput().id);
	}
	usingResource = new ItemInfo();
	newResult = new ItemInfo();

	return S_OK;
}

void StoneFurnace::Release()
{
	SAFE_RELEASE(mainImage);
	SAFE_RELEASE(shadowImage);
	SAFE_RELEASE(fireImage);

	SAFE_DELETE(fuel);
	SAFE_DELETE(resource);
	SAFE_DELETE(usingResource);
	SAFE_DELETE(result);
	SAFE_DELETE(newResult);

	vecRecipes.clear();
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
		// �������� ���� ���ᰡ ���ٸ�
		if (currPower <= 0 && fuel->amount <= 0)
		{
			status = NO_POWER;
			break;
		}
		// ��ᰡ ���� ������� ��ᰡ ���ٸ�
		else if (resource->amount < FindRecipeByIngredient(resource->id)->GetIngredient(0).amount && usingResource->amount <= 0)
		{
			status = NO_RECIPE;
			break;
		}
		// ����
		else
		{
			// ���� �Ϸ��� �ϼ�ǰ�� �����ִٸ�
			if (newResult->amount)
			{
				status = ITEM_PRODUCTION_OVERLOAD;
				break;
			}

			// �������� ���ٸ� ���Ḧ ���
			if (currPower <= 0)
			{
				fuel->amount--;
				currPower += g_itemSpecs[fuel->id].fuelValue;
				maxPower = g_itemSpecs[fuel->id].fuelValue;
			}

			// ������� ��� ���� �� ��� ����
			if (usingResource->amount <= 0)
			{
				currRecipe = FindRecipeByIngredient(resource->id);
				resource->amount -= currRecipe->GetIngredient(0).amount;
				usingResource->amount += currRecipe->GetIngredient(0).amount;
			}

			// ������ �Ҹ�
			currPower -= energyConsumption * TimerManager::GetSingleton()->GetTimeElapsed();
			craftedTime += craftingSpeed * TimerManager::GetSingleton()->GetTimeElapsed();

			// ���� �Ϸ�
			if (craftedTime >= currRecipe->GetCraftingTime())
			{
				usingResource->amount -= currRecipe->GetIngredient(0).amount;
				newResult->id = currRecipe->GetOutput().id;
				newResult->amount = currRecipe->GetOutput().amount;
				craftedTime -= currRecipe->GetCraftingTime();

				status = ITEM_PRODUCTION_OVERLOAD;
			}

			productionPercent = craftedTime / currRecipe->GetCraftingTime();
			time += TimerManager::GetSingleton()->GetTimeElapsed();
		}
		break;

	case StoneFurnace::ITEM_PRODUCTION_OVERLOAD:
		// ������ �ϼ�ǰ�� �ִٸ�
		if (result->amount)
		{
			// ���� ������� �ϼ�ǰ�� ������ �ִ� �ϼ�ǰ�� ���ٸ�
			if (newResult->id == result->id)
			{
				result->amount += newResult->amount;
				newResult->amount = 0;
				status = WORKING;
			}
		}
		// ������ �ϼ�ǰ�� ���ٸ�
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

void StoneFurnace::FirstRender(Shader* lpShader)
{
	shadowImage->Render(lpShader, position.x + 27, position.y - 14);
}

void StoneFurnace::Render(Shader* lpShader)
{
	mainImage->Render(lpShader, position.x, position.y);

	glm::ivec2 maxFrame = fireImage->GetMaxFrame();
	int frame = time * 30;

	int frameX = frame % maxFrame.x;
	int frameY = maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y;

	if(status == WORKING)
		fireImage->Render(lpShader, position.x, position.y, frameX, frameY);
}

bool StoneFurnace::InputItem(ItemInfo* inputItem, glm::vec2 pos)
{
	// �޴� �������� ������
	if (g_itemSpecs[inputItem->id].fuel)
	{
		// ���� ���Կ� ���ᰡ �ִٸ�
		if (fuel->amount)
		{
			// ���� ������ ���� ����
			if (fuel->id == inputItem->id)
			{
				fuel->amount += inputItem->amount;

				SAFE_DELETE(inputItem);
				return true;
			}
		}
		// ���� ���Կ� ���ᰡ ���ٸ�
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
		// ��� ���Կ� ��ᰡ �ִٸ�
		if (resource->amount)
		{
			// ��� ������ ���� ����
			if (resource->id == inputItem->id)
			{
				resource->amount += inputItem->amount;

				SAFE_DELETE(inputItem);
				return true;
			}
		}
		// ��� ������ ��� �ִٸ�
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

void StoneFurnace::ClickEvent()
{
	UIManager::GetSingleton()->ActiveUI("StoneFurnaceUI");
	StoneFurnaceUI* sfUI = (StoneFurnaceUI*)UIManager::GetSingleton()->GetLpCurrUI();
	sfUI->SetCurrStoneFurnace(this);
}

Recipe* StoneFurnace::FindRecipeByIngredient(int itemEnum)
{
	for (int i = 0; i < vecRecipes.size(); i++)
	{
		if (vecRecipes[i]->IsIngredient(itemEnum))
			return vecRecipes[i];
	}
	return nullptr;
}

string StoneFurnace::ToString()
{
	return string("StoneFurnace (") + to_string(coord.x) + string(", ") + to_string(coord.y) + string(") status : ") + to_string(status);
};
