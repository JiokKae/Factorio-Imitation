#include "AssemblingMachine1.h"
#include "selectRecipeAssemblingUI.h"
#include "Recipe.h"

HRESULT AssemblingMachine1::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::ASSEMBLING_MACHINE_1;
	usingClickEvent = true;
	Structure::Init(x, y, direction, temp);

	image = new GLImage();
	image->Init("Entity/AssemblingMachine1", 8, 4);
	image->SetAlpha(1.0f);

	shadow = new GLImage();
	shadow->Init("Entity/AssemblingMachine1-shadow", 8, 4);
	shadow->SetAlpha(0.5f);
	shadowAniOffset = { 16, -6 };

	altModeIcon = new GLImage();
	altModeIcon->Init("Icons/AllItems-stroke", 8, 8, 0.1f, 0.1f);
	altModeIcon->SetScale({ 0.9f, 0.9f });

	craftingSpeed = 0.5f;
	status = NO_RECIPE;

	vecRecipes = RecipeManager::GetSingleton()->FindRecipes(ASSEMBLING_MACHINE_1);
	currRecipeIndex = 0;
	currRecipe = vecRecipes->at(currRecipeIndex);
	ingredients.Init(currRecipe); 

	return S_OK;
}

void AssemblingMachine1::Release()
{
	Structure::Release();

	ingredients.Release();

	SAFE_RELEASE(altModeIcon);
	SAFE_RELEASE(image);
	SAFE_RELEASE(shadow);
}

void AssemblingMachine1::Update()
{
	Structure::Update();

	switch (status)
	{
	case Structure::NO_POWER:
		break;

	case Structure::WORKING:
		if (currRecipe == nullptr)
			status = NO_RECIPE;
		else if (!ingredients.IsEnough(currRecipe))
			status = ITEM_INGREDIENT_SHORTAGE;
		else
		{
			craftedTime += craftingSpeed * TimerManager::GetSingleton()->GetTimeElapsed();

			if (craftedTime >= currRecipe->GetCraftingTime())
			{
				ingredients.Consume(currRecipe);
				result.id = currRecipe->GetOutput().id;
				result.amount += currRecipe->GetOutput().amount;
				productsFinished++;
				craftedTime -= currRecipe->GetCraftingTime();
			}

			productionPercent = craftedTime / currRecipe->GetCraftingTime();
			time += TimerManager::GetSingleton()->GetTimeElapsed();
		}
		break;

	case Structure::ITEM_INGREDIENT_SHORTAGE:
		if (ingredients.IsEnough(currRecipe))
			status = WORKING;
		break;

	case Structure::NO_RECIPE:
		if (currRecipe)
			status = WORKING;
		break;

	}
}

void AssemblingMachine1::FirstRender(Shader* shader)
{
	int frame = (int)(time * 30);
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
	int frame = (int)(time * 30);
	glm::ivec2 maxFrame = image->GetMaxFrame();
	image->Render(shader, posX, posY,
		frame % maxFrame.x, maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y);
}

void AssemblingMachine1::LateRender(Shader* shader)
{
	if(currRecipe)
		altModeIcon->Render(shader, position.x, position.y + 16, currRecipe->GetOutput().id % 8, 7 - currRecipe->GetOutput().id / 8);
}

bool AssemblingMachine1::InputItem(ItemInfo* inputItem, glm::vec2 pos)
{
	if (currRecipe->IsIngredient(inputItem->id))
	{
		for (int i = 0; i < currRecipe->size(); i++)
		{
			if (currRecipe->GetIngredient(i).id == inputItem->id)
			{
				inputItem->MoveAllItemTo(&ingredients[i]);
				SAFE_DELETE(inputItem);
				return true;
			}
		}
	}

	SAFE_DELETE(inputItem);
	return false;
}

bool AssemblingMachine1::TakeOutItem(ItemInfo* outItem)
{
	// 내보낼 생산품이 있다면
	if (!result.IsEmpty())
	{
		// 생산품의 개수가 반출 요구치를 넘으면
		if (result.amount >= outItem->amount)
		{
			result.amount -= outItem->amount;
			outItem->id = result.id;
		}
		// 생산품의 개수가 반출 요구치를 넘지 못하면
		else
			result.MoveAllItemTo(outItem);

		return true;
	}
	// 내보낼 생산품이 없다면
	else
		return false;
}

void AssemblingMachine1::ClickEvent()
{
	if (currRecipe)
	{
		if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LSHIFT))
		{
			currRecipeIndex = (currRecipeIndex + 1) % vecRecipes->size();
			currRecipe = vecRecipes->at(currRecipeIndex);
			ingredients.Init(currRecipe);
			craftedTime = 0;
		}
	}
	else
	{
		UIManager::GetSingleton()->ActiveUI("SelectRecipeAssemblingUI");
		SelectRecipeAssemblingUI* sraUI = (SelectRecipeAssemblingUI*)UIManager::GetSingleton()->GetLpCurrUI();
		//sraUI->SetCurrBurnerMiningDrill(this);
	}
	
}

string AssemblingMachine1::ToString()
{
	char buf[256];
	char buf2[128];
	wsprintf(buf, " \n Crafting Speed: %d \n Recipe: %s", (int)craftingSpeed, g_itemSpecs[currRecipe->GetOutput().id].name.c_str());

	for (size_t i = 0; i < currRecipe->size(); i++)
	{
		wsprintf(buf2, "\n %d/%d x %s", ingredients[i].amount, currRecipe->GetIngredient(i).amount, g_itemSpecs[currRecipe->GetIngredient(i).id].name.c_str());
		strcat_s(buf, buf2);
	}

	if (!result.IsEmpty())
	{
		wsprintf(buf2, "\n Result: %s (%d)", g_itemSpecs[result.id].name.c_str(), result.amount);
		strcat_s(buf, buf2);
	}

	wsprintf(buf2, "\n Products finished: %d", productsFinished);
	strcat_s(buf, buf2);

	return Structure::ToString() + string(buf);
}

HRESULT AssemblingMachine1::Ingredients::Init(Recipe* recipe)
{
	vecIngredients.clear();
	vecIngredients.resize(recipe->size());
	vecUsingIngredients.clear();
	vecUsingIngredients.resize(recipe->size());
	return S_OK;
}

void AssemblingMachine1::Ingredients::Release()
{
	vecIngredients.clear();
}

ItemInfo& AssemblingMachine1::Ingredients::operator[](int index)
{
	return vecIngredients[index];
}

bool AssemblingMachine1::Ingredients::IsEnough(Recipe* recipe)
{
	for (int i = 0; i < vecIngredients.size(); i++)
	{
		// 만약 레시피보다 재료가 적거나 같지 않으면
		if (recipe->GetIngredient(i).amount > vecIngredients[i].amount || recipe->GetIngredient(i).id != vecIngredients[i].id)
			return false;
	}
	return true;
}

bool AssemblingMachine1::Ingredients::Consume(Recipe* recipe)
{
	if (IsEnough(recipe))
	{
		for (size_t i = 0; i < recipe->size(); i++)
		{
			vecIngredients[i].amount -= recipe->GetIngredient(i).amount;
		}
	}
	else
		return false;
}
