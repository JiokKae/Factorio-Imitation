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

	craftingSpeed = 0.5f;
	status = NO_RECIPE;
	selectedRecipe = RecipeManager::GetSingleton()->FindRecipe(IRON_GEAR_WHEEL);
	ingredients.Init(selectedRecipe); 

	return S_OK;
}

void AssemblingMachine1::Release()
{
	Structure::Release();

	ingredients.Release();
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
		if (selectedRecipe == nullptr)
			status = NO_RECIPE;
		else if (!ingredients.IsEnough(selectedRecipe))
			status = ITEM_INGREDIENT_SHORTAGE;
		else
		{
			craftedTime += craftingSpeed * TimerManager::GetSingleton()->GetTimeElapsed();

			if (craftedTime >= selectedRecipe->GetCraftingTime())
			{
				result.id = selectedRecipe->GetOutput().id;
				result.amount += selectedRecipe->GetOutput().amount;
				craftedTime -= selectedRecipe->GetCraftingTime();
			}

			productionPercent = craftedTime / selectedRecipe->GetCraftingTime();
			time += TimerManager::GetSingleton()->GetTimeElapsed();
		}
		break;

	case Structure::ITEM_INGREDIENT_SHORTAGE:
		if (ingredients.IsEnough(selectedRecipe))
			status = WORKING;
		break;

	case Structure::NO_RECIPE:
		if (selectedRecipe)
			status = WORKING;
		break;

	}
}

void AssemblingMachine1::FirstRender(Shader* shader)
{
	int frame = g_time * 30;
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
	int frame = time * 30;
	glm::ivec2 maxFrame = image->GetMaxFrame();
	image->Render(shader, posX, posY,
		frame % maxFrame.x, maxFrame.y - 1 - frame / maxFrame.x % maxFrame.y);
}

bool AssemblingMachine1::InputItem(ItemInfo* inputItem, glm::vec2 pos)
{
	if (selectedRecipe->IsIngredient(inputItem->id))
	{
		for (int i = 0; i < selectedRecipe->size(); i++)
		{
			if (selectedRecipe->GetIngredient(i).id == inputItem->id)
			{
				inputItem->MoveItemTo(&ingredients[i]);
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
	// ������ ����ǰ�� �ִٸ�
	if (result.amount)
	{
		// ����ǰ�� ������ ���� �䱸ġ�� ������
		if (result.amount >= outItem->amount)
		{
			result.amount -= outItem->amount;
			outItem->id = result.id;
		}
		// ����ǰ�� ������ ���� �䱸ġ�� ���� ���ϸ�
		else
		{
			outItem->id = result.id;
			outItem->amount = result.amount;
			result.amount = 0;
		}
		return true;
	}
	// ������ ����ǰ�� ���ٸ�
	else
		return false;
}

void AssemblingMachine1::ClickEvent()
{
	if (selectedRecipe)
	{
		
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
	char buf[128];
	wsprintf(buf, "\n Recipe: %s \n Result: %s (%d)",
		g_itemSpecs[selectedRecipe->GetOutput().id].name.c_str(), 
		g_itemSpecs[result.id].name.c_str(), result.amount);
	return Structure::ToString() + string(buf);
}

HRESULT AssemblingMachine1::Ingredients::Init(Recipe* recipe)
{
	vecIngredients.resize(recipe->size());
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
		// ���� �����Ǻ��� ��ᰡ ���ų� ���� ������
		if (recipe->GetIngredient(i).amount > vecIngredients[i].amount || recipe->GetIngredient(i).id != vecIngredients[i].id)
			return false;
	}
	return true;
}

bool AssemblingMachine1::Ingredients::Consume(Recipe* recipe)
{
	if (IsEnough(recipe))
	{

	}
	else
		return false;
}
