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
	ingredients.resize(selectedRecipe->size());

	return S_OK;
}

void AssemblingMachine1::Release()
{
	Structure::Release();

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
		if (!selectedRecipe->IsEnoughIngredient(&ingredients))
			status = ITEM_INGREDIENT_SHORTAGE;
		break;

	case Structure::ITEM_INGREDIENT_SHORTAGE:
		if (selectedRecipe->IsEnoughIngredient(&ingredients))
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
	wsprintf(buf, "\n Recipe: %s", g_itemSpecs[selectedRecipe->GetOutput().id].name.c_str());
	return Structure::ToString() + string(buf);
}
