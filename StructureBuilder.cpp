#include "StructureBuilder.h"
#include "Shader.h"
#include "ItemEnum.h"
#include "GLImage.h"
#include "EntityManager.h"
#include "Tile.h"
#include "HandUI.h"
#include "Texture.h"
#include "Structure.h"
HRESULT StructureBuilder::Init(EntityManager* entityManager)
{
    this->entityManager = entityManager;
    itemBuildImage = new GLImage();
    entityDirection = DIRECTION::NORTH;
    canBuildDistance = 700.0f;
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

    ItemSpec spec = g_itemSpecs[itemId];
    // °¡·Î°¡ È¦¼ö Ä­ÀÌ¸é
    if (spec.coordSize.x & 1)
    {
        if (g_cursorPosition.x < 0)
            buildPos.x = int(g_cursorPosition.x) / TILE_SIZE * TILE_SIZE - TILE_SIZE / 2 + 1;
        else
            buildPos.x = int(g_cursorPosition.x) / TILE_SIZE * TILE_SIZE + TILE_SIZE / 2;
    }
    // Â¦¼ö Ä­ÀÌ¸é
    else
    {
        if (g_cursorPosition.x < 0)
            buildPos.x = int(g_cursorPosition.x - TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE + 1;
        else
            buildPos.x = int(g_cursorPosition.x + TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE;
    }

    // ¼¼·Î°¡ È¦¼ö Ä­ÀÌ¸é
    if (spec.coordSize.y & 1)
    {
        if (g_cursorPosition.y < 0)
            buildPos.y = int(g_cursorPosition.y) / TILE_SIZE * TILE_SIZE - TILE_SIZE / 2 + 1;
        else
            buildPos.y = int(g_cursorPosition.y) / TILE_SIZE * TILE_SIZE + TILE_SIZE / 2;
    }
    // Â¦¼ö Ä­ÀÌ¸é
    else
    {
        if (g_cursorPosition.y < 0)
            buildPos.y = int(g_cursorPosition.y - TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE + 1;
        else
            buildPos.y = int(g_cursorPosition.y + TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE;
    }

    ivec2 buildCoord = POS_TO_COORD(buildPos);
    if (distance((vec2)buildPos, *playerPos) < canBuildDistance && CheckCanBuild(buildCoord, spec.coordSize))
    {
        buildable = true;
        if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
        {
            SoundManager::GetSingleton()->Play("Build-medium", 0.6f);
            Structure* structure = Structure::CreateStructure((ItemEnum)itemId);
            structure->Init(buildPos.x, buildPos.y);
            entityManager->AddEntity(structure);
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

void StructureBuilder::Active(int itemId)
{
    active = true;
    this->itemId = itemId;
    ItemSpec spec = g_itemSpecs[itemId];

    entityDirection = DIRECTION::NORTH;
    if(spec.directionCount > 1)
        itemBuildImage->Init(string("Entity/" + spec.name + g_directionToLpChar[entityDirection]).c_str(), spec.maxFrame.x, spec.maxFrame.y);
    else
        itemBuildImage->Init(string("Entity/" + spec.name).c_str(), spec.maxFrame.x, spec.maxFrame.y);
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
