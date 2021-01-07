#include "StructureBuilder.h"
#include "Shader.h"
#include "ItemEnum.h"
#include "GLImage.h"
#include "BurnerMiningDrill.h"
#include "EntityManager.h"
#include "Tile.h"

HRESULT StructureBuilder::Init(EntityManager* entityManager)
{
    this->entityManager = entityManager;
    itemBuildImage = new GLImage();
    entityName = "BurnerMiningDrill";
    entityDirection = DIRECTION::NORTH;
    itemBuildImage->Init(string("Entity/" + entityName + g_directionToLpChar[entityDirection]).c_str(), 4, 8);
    canBuildDistance = 800.0f;
    return S_OK;
}

void StructureBuilder::Release()
{
    SAFE_RELEASE(itemBuildImage);
}

void StructureBuilder::Update(vec2* playerPos)
{
    if (!active)
        return;

    if (g_cursorPosition.x < 0)
        buildPos.x = int(g_cursorPosition.x - TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE;
    else
        buildPos.x = int(g_cursorPosition.x + TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE;

    if (g_cursorPosition.y < 0)
        buildPos.y = int(g_cursorPosition.y - TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE;
    else
        buildPos.y = int(g_cursorPosition.y + TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE;

    ivec2 buildCoord = POS_TO_COORD(buildPos);
    if (distance((vec2)buildPos, *playerPos) < canBuildDistance && CheckCanBuild(buildCoord, g_itemSpecs[ItemEnum::BURNER_MINING_DRILL].coordSize))
    {
        buildable = true;
        if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
        {
            SoundManager::GetSingleton()->Play("Build-medium", 0.6f);
            BurnerMiningDrill* drill = new BurnerMiningDrill();
            drill->Init(buildPos.x, buildPos.y);
            entityManager->AddEntity(drill);
        }
    }
    else
    {
        buildable = false;
    }
}

void StructureBuilder::Render(Shader* shader)
{
    if (!active)
        return;

    if (buildable)
        shader->setVec3("material.diffuseColor", vec3(0.0f, 1.0f, 0.0f));
    else
        shader->setVec3("material.diffuseColor", vec3(1.0f, 0.0f, 0.0f));

    itemBuildImage->Render(shader, buildPos.x, buildPos.y);

    shader->setVec3("material.diffuseColor", vec3(1.0f, 1.0f, 1.0f));
}

void StructureBuilder::Active()
{
    active = true;
}

void StructureBuilder::Deactive()
{
    active = false;
}

bool StructureBuilder::CheckCanBuild(ivec2 coord, ivec2 coordSize)
{
    for (int y = 0; y < coordSize.y; y++)
    {
        for (int x = 0; x < coordSize.x; x++)
        {
            Tile* tile = TileManager::GetSingleton()->GetLpTile(coord.x - coordSize.x / 2 * ((coord.x < 0) ? 0 : 1) + x, coord.y - coordSize.y / 2 * ((coord.y < 0) ? 0 : 1) + y);

            if (!tile || tile->GetLpSturcture() != nullptr)
                return false;
        }
    }
    return true;
}
