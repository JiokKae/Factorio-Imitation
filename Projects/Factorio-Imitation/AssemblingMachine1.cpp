#include "AssemblingMachine1.h"
#include "selectRecipeAssemblingUI.h"
#include "Recipe.h"

HRESULT AssemblingMachine1::Init(int x, int y, DIRECTION _direction, bool _temp)
{
	itemId = ItemId::ASSEMBLING_MACHINE_1;
	usingClickEvent = true;
	Structure::Init(x, y, _direction, _temp);

	mainImage = new GLImage("Entity/AssemblingMachine1", 8, 4);
	mainImage->SetAlpha(1.0f);

	shadowImage = new GLImage("Entity/AssemblingMachine1-shadow", 8, 4);
	shadowImage->SetAlpha(0.5f);
	shadowImageOffset = { 16, -6 };

	altModeIcon = new GLImage("Icons/AllItems-stroke", 8, 8, 0.1f, 0.1f);
	altModeIcon->SetScale({ 0.9f, 0.9f });

	craftingSpeed = 0.5f;
	status = NO_RECIPE;

	vecRecipes = RecipeManager::GetSingleton()->FindRecipePreset(ASSEMBLING_MACHINE_1);
	currRecipeIndex = 0;
	currRecipe = vecRecipes->at(currRecipeIndex);
	ingredients.Init(currRecipe); 

	return S_OK;
}

void AssemblingMachine1::Release()
{
	Structure::Release();

	ingredients.Release();

	SAFE_DELETE(altModeIcon);
	SAFE_DELETE(mainImage);
	SAFE_DELETE(shadowImage);
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

void AssemblingMachine1::FirstRender(ShaderProgram* shader)
{
	int frame = (int)(time * 30);
	glm::ivec2 maxFrame = mainImage->GetMaxFrame();

	shadowImage->Render(shader, position.x + shadowImageOffset.x, position.y + shadowImageOffset.y,
		frame % maxFrame.x, maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y);
}

void AssemblingMachine1::Render(ShaderProgram* shader)
{
	int frame = (int)(time * 30);
	glm::ivec2 maxFrame = mainImage->GetMaxFrame();

	mainImage->Render(shader, position.x, position.y,
		frame % maxFrame.x, maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y);
}

void AssemblingMachine1::RenderInScreen(ShaderProgram* shader, float posX, float posY)
{
	int frame = (int)(time * 30);
	glm::ivec2 maxFrame = mainImage->GetMaxFrame();

	mainImage->Render(shader, posX, posY,
		frame % maxFrame.x, maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y);
}

void AssemblingMachine1::LateRender(ShaderProgram* shader)
{
	if(currRecipe)
		altModeIcon->Render(shader, position.x, position.y + 16, currRecipe->GetOutput().id % 8, 7 - currRecipe->GetOutput().id / 8);
}

bool AssemblingMachine1::InputItem(ItemInfo* inputItem, glm::vec2 /*position*/)
{
	if (currRecipe->IsIngredient(inputItem->id) == false)
	{
		return false;
	}

	for (int i = 0; i < currRecipe->GetIngredients().size(); i++)
	{
		if (currRecipe->GetIngredients()[i].id == inputItem->id)
		{
			inputItem->MoveAllItemTo(&ingredients[i]);
			return true;
		}
	}
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
		//SelectRecipeAssemblingUI* sraUI = (SelectRecipeAssemblingUI*)UIManager::GetSingleton()->GetLpCurrUI();
		//sraUI->SetCurrBurnerMiningDrill(this);
	}
	
}

string AssemblingMachine1::ToString()
{
	//TODO: 제대로 출력되는지 확인
	//char buf[256];
	//char buf2[128];
	std::string bufstr = std::format(" \n Crafting Speed: {} \n Recipe: {}", (int)craftingSpeed, g_itemSpecs[currRecipe->GetOutput().id].name.c_str());
	//wsprintf(buf, " \n Crafting Speed: %d \n Recipe: %s", );

	for (size_t i = 0; i < currRecipe->GetIngredients().size(); i++)
	{
		std::string bufstr2 = std::format("\n {}/{} x {}", ingredients[i].amount, currRecipe->GetIngredients()[i].amount, g_itemSpecs[currRecipe->GetIngredients()[i].id].name.c_str());
		bufstr += bufstr2;
		//wsprintf(buf2, "\n %d/%d x %s", ingredients[i].amount, currRecipe->GetIngredient(i).amount, g_itemSpecs[currRecipe->GetIngredient(i).id].name.c_str());
		//strcat_s(buf, buf2);
	}

	if (!result.IsEmpty())
	{
		std::string bufstr2 = std::format("\n Result: {} ({})", g_itemSpecs[result.id].name.c_str(), result.amount);
		bufstr += bufstr2;
		//wsprintf(buf2, "\n Result: %s (%d)", g_itemSpecs[result.id].name.c_str(), result.amount);
		//strcat_s(buf, buf2);
	}
	std::string bufstr2 = std::format("\n Products finished: {}", (int)productsFinished);
	bufstr += bufstr2;
	//wsprintf(buf2, "\n Products finished: %d", (int)productsFinished);
	//strcat_s(buf, buf2);

	return Structure::ToString() + bufstr;
}

HRESULT AssemblingMachine1::Ingredients::Init(Recipe* recipe)
{
	vecIngredients.clear();
	vecIngredients.resize(recipe->GetIngredients().size());
	vecUsingIngredients.clear();
	vecUsingIngredients.resize(recipe->GetIngredients().size());
	return S_OK;
}

void AssemblingMachine1::Ingredients::Release()
{
	vecIngredients.clear();
}

ItemInfo& AssemblingMachine1::Ingredients::operator[](size_t index)
{
	return vecIngredients[index];
}

bool AssemblingMachine1::Ingredients::IsEnough(Recipe* recipe)
{
	for (int i = 0; i < vecIngredients.size(); i++)
	{
		// 만약 레시피보다 재료가 적거나 같지 않으면
		if (recipe->GetIngredients()[i].amount > vecIngredients[i].amount || recipe->GetIngredients()[i].id != vecIngredients[i].id)
			return false;
	}
	return true;
}

bool AssemblingMachine1::Ingredients::Consume(Recipe* recipe)
{
	if (IsEnough(recipe))
	{
		for (size_t i = 0; i < recipe->GetIngredients().size(); i++)
		{
			vecIngredients[i].amount -= recipe->GetIngredients()[i].amount;
		}
		return true;
	}
	else
		return false;
}
