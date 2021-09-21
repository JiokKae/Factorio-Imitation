#include "TileManager.h"
#include "Tile.h"
#include "../GLFramework/Image/GLImage.h"
#include "../GLFramework/Shader/Shader.h"
#include "../GLFramework/Texture/Texture.h"
#include "Ore.h"
#include "Chunk.h"
#include "../GLFramework/VertexDataObject/VertexArrayObject.h"
#include "../GLFramework/VertexDataObject/VertexBufferObject.h"
#include "ItemEnum.h"

HRESULT TileManager::Init()
{
    tileImages = new GLImage[int(Tile::KIND::END)]();
    tileImages[int(Tile::KIND::DIRT_1)].Init("Terrain/Dirt_1", 4096, 576, 64, 9);
    
    mapOreImages[COAL] = new GLImage();
    mapOreImages[COAL]->Init("Entity/Coal", 8, 8);
    mapOreImages[IRON_ORE] = new GLImage();
    mapOreImages[IRON_ORE]->Init("Entity/IronOre", 8, 8);
    mapOreImages[COPPER_ORE] = new GLImage();
    mapOreImages[COPPER_ORE]->Init("Entity/CopperOre", 8, 8);
    mapOreImages[STONE] = new GLImage();
    mapOreImages[STONE]->Init("Entity/Stone", 8, 8);

    // 14 * 14 청크 생성
    for (int y = -7; y < 7; y++)
    {
        for (int x = -7; x < 7; x++)
        {
            mapChunks[y][x] = new Chunk();
            mapChunks[y][x]->Init(x, y);
        }
    }

    // 인스턴싱 쉐이더 생성 및 초기화
    instancingShader = new Shader("InstancingVertexShader.glsl", "StandardFragmentShader.glsl");
    instancingShader->use();
    instancingShader->setInt("material.diffuse", 0);
    instancingShader->setVec3("material.diffuseColor", glm::vec3(1.0f, 1.0f, 1.0f));
    instancingShader->setFloat("alpha", 1.0f);
    instancingShader->setFloat("vertexScale", 1.0f);
    instancingShader->setMat4("model", glm::mat4());
    
    // 아래의 VBO들을 담는 Vertex Array Object를 생성한다.
    tilesVAO = new VAO();

    // tileQuadVBO Init (사각형 버텍스들을 저장하는 VBO)
    VBO* tileQuadVBO = new VBO();   // 사각형 버텍스들을 저장하는 VBO
    float vertices[] = {
        // positions       // texture coords
        -64 / 2, -64 / 2,  0.0f, 0.0f,
         64 / 2, -64 / 2,  1.0f, 0.0f,
         64 / 2,  64 / 2,  1.0f, 1.0f,
         64 / 2,  64 / 2,  1.0f, 1.0f,
        -64 / 2,  64 / 2,  0.0f, 1.0f,
        -64 / 2, -64 / 2,  0.0f, 0.0f,
    };
    tileQuadVBO->SetData(sizeof(float) * 4 * 6, vertices, GL_STATIC_DRAW); // vec4(vec2 pos, vec2 texCoord) * 6
    tilesVAO->AddVBO(0, tileQuadVBO, 4);

    // currFrameVBO Init (각 인스턴스의 현재 프레임을 저장하는 VBO)
    VBO* currFrameVBO = new VBO();
    currFrameVBO->SetData(sizeof(glm::vec2) * 1024, NULL, GL_DYNAMIC_DRAW); // vec2(currFrame.x, currFrame.y) * 1024
    tilesVAO->AddVBO(1, currFrameVBO, 2);
    tilesVAO->SetDivisor(1, 1);
    
    // offsetVBO Init (각 인스턴스의 위치 오프셋을 저장하는 VBO)
    VBO* offsetVBO = new VBO();
    offsetVBO->SetData(sizeof(glm::vec2) * 1024, NULL, GL_DYNAMIC_DRAW); // vec2(currFrame.x, currFrame.y) * 1024
    tilesVAO->AddVBO(2, offsetVBO, 2);
    tilesVAO->SetDivisor(2, 1);

    tileCurrFrame = new glm::vec2[CHUNK_IN_TILE * CHUNK_IN_TILE];
    tileOffset = new glm::vec2[CHUNK_IN_TILE * CHUNK_IN_TILE];
    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 32; x++)
        {
            tileCurrFrame[y * CHUNK_IN_TILE + x] = { x % 64, y % 4 };
            /*
            Tile* tile = currChunk->GetLpTile(x, y);
            if(tile->GetLpSturcture() != nullptr)
                tileCurrFrame[y * CHUNK_IN_TILE + x] = { x % 64, y % 4 };
            else
                tileCurrFrame[y * CHUNK_IN_TILE + x] = { 100, 100 };
            */
            tileOffset[y * CHUNK_IN_TILE + x] = { x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2 };
        }
    }
	return S_OK;
}

void TileManager::Release()
{
    SAFE_DELETE(tilesVAO);
    SAFE_DELETE(instancingShader);
    SAFE_RELEASE(tileImages);

    for (auto it = mapOreImages.begin(); it != mapOreImages.end(); ++it)
    {
        SAFE_RELEASE(it->second);
    }
    mapOreImages.clear();

    SAFE_ARR_DELETE(tileOffset);
    SAFE_ARR_DELETE(tileCurrFrame);
    
    ReleaseSingleton();
}

void TileManager::Update()
{

}

void TileManager::Render(RECT cameraRect)
{
    vecChunkInScreen.clear();
    for (bigIt = mapChunks.begin(); bigIt != mapChunks.end(); bigIt++)
    {
        for (smallIt = bigIt->second.begin(); smallIt != bigIt->second.end(); smallIt++)
        {
            Chunk* currChunk = smallIt->second;
            if (currChunk == nullptr)
                continue;
            if (CheckRectCollision(currChunk->GetRect(), cameraRect))
                vecChunkInScreen.push_back(currChunk);
        }
    }

    instancingShader->use();
    glBindVertexArray(tilesVAO->GetID());
    instancingShader->setFloat("vertexScale", 1.0f);
    instancingShader->setVec2("maxFrame", glm::vec2(64, 9));
    for (vecChunkIt = vecChunkInScreen.begin() ; vecChunkIt != vecChunkInScreen.end(); ++vecChunkIt)
    {
        instancingShader->setVec2("offset", (*vecChunkIt)->GetCoord() * 2048);

        tilesVAO->SetVBOData(1, sizeof(glm::vec2) * 1024, &tileCurrFrame[0], GL_DYNAMIC_DRAW);
        tilesVAO->SetVBOData(2, sizeof(glm::vec2) * 1024, &tileOffset[0], GL_DYNAMIC_DRAW);
        for (int kind = 0; kind < int(Tile::KIND::END); kind++)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tileImages[kind].GetLpSourceTexture()->GetID());
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1024);
        }
    }
    DebugHelper::GetSingleton()->SetFloat("2.1. tileRender", TimerManager::GetSingleton()->CheckTime());

    instancingShader->setFloat("vertexScale", 2.0f);
    instancingShader->setVec2("maxFrame", glm::vec2(8, 8));
    for (vecChunkIt = vecChunkInScreen.begin(); vecChunkIt != vecChunkInScreen.end(); ++vecChunkIt)
    {
        instancingShader->setVec2("offset", (*vecChunkIt)->GetCoord() * 2048);

        map<int, GLImage*>::iterator it;
        for (it = mapOreImages.begin(); it != mapOreImages.end(); ++it)
        {
            int count = 0;

            for (int y = 0; y < 32; y++)
            {
                for (int x = 0; x < 32; x++)
                {
                    Ore* ore = (*vecChunkIt)->GetLpTile(x, y)->GetLpOre();
                    if (ore->GetItemEnum() == it->first && ore->GetAmount() > 0)
                    {
                        oreCurrFrame[count] = { ore->GetRandFrameX(), ore->GetFrameY() };
                        oreOffset[count] = { x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2 };
                        count++;
                    }
                }
            }
            tilesVAO->SetVBOData(1, sizeof(glm::vec2) * count, &oreCurrFrame[0], GL_DYNAMIC_DRAW);
            tilesVAO->SetVBOData(2, sizeof(glm::vec2) * count, &oreOffset[0], GL_DYNAMIC_DRAW);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, it->second->GetLpSourceTexture()->GetID());
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
        }
    }
    DebugHelper::GetSingleton()->SetFloat("2.2. oreRender", TimerManager::GetSingleton()->CheckTime());

    glBindVertexArray(0);
}

Tile* TileManager::GetLpTile(int coordX, int coordY)
{
    glm::ivec2 chunkCoord;

    chunkCoord.x = TILECOORDX_TO_CHUNKCOORDX(coordX + ((coordX < -1) ? 1 : 0));
    chunkCoord.y = TILECOORDX_TO_CHUNKCOORDX(coordY + ((coordY < -1) ? 1 : 0));

    Chunk* chunk = GetLpChunk(chunkCoord.x, chunkCoord.y);
    if (chunk != nullptr)
    {
        glm::ivec2 indexInChunk;
        indexInChunk.x = coordX % CHUNK_IN_TILE;
        indexInChunk.y = coordY % CHUNK_IN_TILE;

        if (indexInChunk.x < 0)
            indexInChunk.x += CHUNK_IN_TILE;
        if (indexInChunk.y < 0)
            indexInChunk.y += CHUNK_IN_TILE;

        return chunk->GetLpTile(indexInChunk.x, indexInChunk.y);
    }
    else
        return nullptr;
}

Tile* TileManager::GetLpTile(float positionX, float positionY)
{
    glm::vec2 position = { positionX, positionY };
    glm::ivec2 coord = POS_TO_COORD(position);
    return GetLpTile(coord.x, coord.y);
}

Tile* TileManager::GetLPTileUnderMouse()
{
    return GetLpTile(g_cursorCoord.x, g_cursorCoord.y);
}

Chunk* TileManager::GetLpChunk(int x, int y)
{
    return mapChunks[y][x];
}
