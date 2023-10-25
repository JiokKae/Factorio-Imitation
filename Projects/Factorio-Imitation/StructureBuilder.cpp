#include "StructureBuilder.h"
#include "EntityManager.h"
#include "HandUI.h"
#include "ItemEnum.h"
#include "Structure.h"
#include "Tile.h"
#include "../GLFramework/Image/GLImage.h"
#include "../GLFramework/ShaderProgram/ShaderProgram.h"
#include "../GLFramework/Texture/Texture.h"

HRESULT StructureBuilder::Init()
{
	canBuildDistance = 700.0f;
	return S_OK;
}

void StructureBuilder::Release()
{
}

void StructureBuilder::Update(vec2* playerPos)
{
	if (active == false)
	{
		return;
	}

	const ItemSpec& spec = g_itemSpecs[itemId];

	glm::ivec2 buildPos;

	if (IsOddNumber(spec.coordSize.x) == true)
	{
		if (g_cursorPosition.x < 0)
			buildPos.x = int(g_cursorPosition.x) / TILE_SIZE * TILE_SIZE - TILE_SIZE / 2 + 1;
		else
			buildPos.x = int(g_cursorPosition.x) / TILE_SIZE * TILE_SIZE + TILE_SIZE / 2;
	}
	else
	{
		if (g_cursorPosition.x < 0)
			buildPos.x = int(g_cursorPosition.x - TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE + 1;
		else
			buildPos.x = int(g_cursorPosition.x + TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE;
	}

	if (IsOddNumber(spec.coordSize.y) == true)
	{
		if (g_cursorPosition.y < 0)
			buildPos.y = int(g_cursorPosition.y) / TILE_SIZE * TILE_SIZE - TILE_SIZE / 2 + 1;
		else
			buildPos.y = int(g_cursorPosition.y) / TILE_SIZE * TILE_SIZE + TILE_SIZE / 2;
	}
	else
	{
		if (g_cursorPosition.y < 0)
			buildPos.y = int(g_cursorPosition.y - TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE + 1;
		else
			buildPos.y = int(g_cursorPosition.y + TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE;
	}

	tempStructure->SetPosition(Vec2(buildPos.x, buildPos.y));
	tempStructure->Update();

	glm::ivec2 buildCoord(POS_TO_COORD(buildPos));

	buildable = glm::distance((glm::vec2)buildPos, *playerPos) < canBuildDistance && CheckCanBuild(buildCoord, spec.coordSize);

	if (buildable)
	{
		if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
		{
			SoundManager::GetSingleton()->Play(GetBuildSoundName(tempStructure->GetCoordSize()), 0.6f);

			UIManager::GetSingleton()->GetLpHandUI()->GetHandItem()->amount--;
			Structure* structure = Structure::CreateStructure((ItemEnum)itemId);

			structure->Init(buildPos.x, buildPos.y, tempStructure->GetDirection());
			EntityManager::GetSingleton()->AddEntity(structure);

			KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON);
		}
	}
}

void StructureBuilder::Render(ShaderProgram* shader)
{
	if (!active)
		return;

	if (buildable)
		shader->setVec3("material.diffuseColor", vec3(0.0f, 1.0f, 0.0f));
	else
		shader->setVec3("material.diffuseColor", vec3(1.0f, 0.0f, 0.0f));

	tempStructure->FirstRender(shader);
	tempStructure->Render(shader);
	tempStructure->LateRender(shader);

	shader->setVec3("material.diffuseColor", vec3(1.0f, 1.0f, 1.0f));
}

void StructureBuilder::Activate(int _itemId)
{
	active = true;
	itemId = _itemId;

	if (tempStructure)
	{
		if (tempStructure->GetItemId() == itemId)
		{
			return;
		}

		SAFE_RELEASE(tempStructure);
	}

	tempStructure = Structure::CreateStructure((ItemEnum)itemId);
	tempStructure->Init(0, 0, NORTH, true);
}

void StructureBuilder::Deactivate()
{
	active = false;
}

bool StructureBuilder::CheckCanBuild(ivec2 coord, ivec2 coordSize)
{
	for (int y = 0; y < coordSize.y; y++)
	{
		for (int x = 0; x < coordSize.x; x++)
		{
			Tile* tile = TileManager::GetSingleton()->GetLpTile(
				coord.x - coordSize.x / 2 + x,
				coord.y - coordSize.y / 2 + y
			);
			if (!tile || tile->GetLpSturcture() != nullptr)
				return false;
		}
	}
	return true;
}

bool StructureBuilder::IsOddNumber(int num) const
{
	return num & 1;
}

std::string StructureBuilder::GetBuildSoundName(const glm::ivec2& coordSize) const
{
	if (coordSize.x + coordSize.y > 4)
		return std::string("Build-large");

	if (coordSize.x + coordSize.y > 2)
		return std::string("Build-medium");

	return std::string("Build-small");
}
