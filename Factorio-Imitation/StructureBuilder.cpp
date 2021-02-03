#include "StructureBuilder.h"
#include "Shader.h"
#include "ItemEnum.h"
#include "GLImage.h"
#include "EntityManager.h"
#include "Tile.h"
#include "HandUI.h"
#include "Texture.h"
#include "Structure.h"
HRESULT StructureBuilder::Init()
{
    this->entityManager = EntityManager::GetSingleton();
    canBuildDistance = 700.0f;
    return S_OK;
}

void StructureBuilder::Release()
{
}

void StructureBuilder::Update(vec2* playerPos)
{
    if (!active)
        return;

    ItemSpec spec = g_itemSpecs[itemId];
    // 가로가 홀수 칸이면
    if (spec.coordSize.x & 1)
    {
        if (g_cursorPosition.x < 0)
            buildPos.x = int(g_cursorPosition.x) / TILE_SIZE * TILE_SIZE - TILE_SIZE / 2 + 1;
        else
            buildPos.x = int(g_cursorPosition.x) / TILE_SIZE * TILE_SIZE + TILE_SIZE / 2;
    }
    // 짝수 칸이면
    else
    {
        if (g_cursorPosition.x < 0)
            buildPos.x = int(g_cursorPosition.x - TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE + 1;
        else
            buildPos.x = int(g_cursorPosition.x + TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE;
    }

    // 세로가 홀수 칸이면
    if (spec.coordSize.y & 1)
    {
        if (g_cursorPosition.y < 0)
            buildPos.y = int(g_cursorPosition.y) / TILE_SIZE * TILE_SIZE - TILE_SIZE / 2 + 1;
        else
            buildPos.y = int(g_cursorPosition.y) / TILE_SIZE * TILE_SIZE + TILE_SIZE / 2;
    }
    // 짝수 칸이면면
    else
    {
        if (g_cursorPosition.y < 0)
            buildPos.y = int(g_cursorPosition.y - TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE + 1;
        else
            buildPos.y = int(g_cursorPosition.y + TILE_SIZE / 2) / TILE_SIZE * TILE_SIZE;
    }

    tempStructure->SetPosition( Vec2(buildPos.x, buildPos.y) );
    tempStructure->Update();
    ivec2 buildCoord = ivec2(POS_TO_COORD(buildPos));
    if (distance((vec2)buildPos, *playerPos) < canBuildDistance && CheckCanBuild(buildCoord, spec.coordSize))
    {
        buildable = true;
        if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
        {
            // build sound
            if(tempStructure->GetCoordSize().x + tempStructure->GetCoordSize().y > 4)
                SoundManager::GetSingleton()->Play("Build-large", 0.6f);
            else if (tempStructure->GetCoordSize().x + tempStructure->GetCoordSize().y > 2)
                SoundManager::GetSingleton()->Play("Build-medium", 0.6f);
            else
                SoundManager::GetSingleton()->Play("Build-small", 0.6f);
            
            UIManager::GetSingleton()->GetLpHandUI()->GetHandItem()->amount--;
            Structure* structure = Structure::CreateStructure((ItemEnum)itemId);
           
            structure->Init(buildPos.x, buildPos.y, tempStructure->GetDirection());
            entityManager->AddEntity(structure);

            KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON);
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

    tempStructure->FirstRender(shader);
    tempStructure->Render(shader);
    tempStructure->LateRender(shader);

    shader->setVec3("material.diffuseColor", vec3(1.0f, 1.0f, 1.0f));
}

void StructureBuilder::Active(int itemId)
{
    active = true;
    this->itemId = itemId;
    ItemSpec spec = g_itemSpecs[itemId];

    if (tempStructure)
    {
        if (tempStructure->GetItemId() != itemId)
        {
            SAFE_RELEASE(tempStructure);
            tempStructure = Structure::CreateStructure((ItemEnum)itemId);
            tempStructure->Init(0, 0, NORTH, true);
        }
    }
    else
    {
        tempStructure = Structure::CreateStructure((ItemEnum)itemId);
        tempStructure->Init(0, 0, NORTH, true);
    }
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
