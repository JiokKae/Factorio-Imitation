#include "TileManager.h"
#include "Tile.h"
#include "GLImage.h"
#include "Shader.h"
#include "Texture.h"
#include "Ore.h"
#include "Chunk.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

HRESULT TileManager::Init()
{
    tileImages = new GLImage[int(Tile::KIND::END)]();
    tileImages[int(Tile::KIND::DIRT_1)].Init("Terrain/Dirt_1", 4096, 576, 64, 9);
    
    oreImages = new GLImage[int(Ore::KIND::END)]();
    oreImages[int(Ore::KIND::IRON_ORE)].Init("IronOre", 1024, 1024, 8, 8);
    
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
    instancingShader->setFloat("alpha", 1.0f);
    instancingShader->setMat4("model", glm::mat4());
    
    // 아래의 VBO들을 담는 Vertex Array Object를 생성한다.
    tilesVAO = new VAO();

    // tileQuadVBO Init (사각형 버텍스들을 저장하는 VBO)
    VBO* tileQuadVBO = new VBO();   // 사각형 버텍스들을 저장하는 VBO
    tileQuadVBO->SetData(sizeof(float) * 4 * 6, NULL, GL_DYNAMIC_DRAW); // vec4(vec2 pos, vec2 texCoord) * 6
    tilesVAO->AddVBO(0, tileQuadVBO, 4);

    // currFrameVBO Init (각 인스턴스의 현재 프레임을 저장하는 VBO)
    VBO* currFrameVBO = new VBO();
    currFrameVBO->SetData(sizeof(glm::vec2) * 1024, NULL, GL_DYNAMIC_DRAW); // vec2(currFrame.x, currFrame.y) * 1024
    tilesVAO->AddVBO(1, currFrameVBO, 2);
    tilesVAO->SetDivisor(1, 1);
    
    // offsetVBO Init (각 인스턴스의 위치 오프셋을 저장하는 VBO)

    //set Data
    glm::vec2* tileOffset = new glm::vec2[CHUNK_IN_TILE * CHUNK_IN_TILE];
    for (int y = 0; y < CHUNK_IN_TILE; y++)
        for (int x = 0; x < CHUNK_IN_TILE; x++)
            tileOffset[y * CHUNK_IN_TILE + x] = { x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2 }; 
    
    VBO* offsetVBO = new VBO();
    offsetVBO->SetData(sizeof(glm::vec2) * 1024, &tileOffset[0], GL_STATIC_DRAW); // vec2(currFrame.x, currFrame.y) * 1024
    tilesVAO->AddVBO(2, offsetVBO, 2);
    tilesVAO->SetDivisor(2, 1);

    delete[] tileOffset;

	return S_OK;
}

void TileManager::Release()
{
    SAFE_DELETE(tilesVAO);
    SAFE_DELETE(instancingShader);
    SAFE_RELEASE(tileImages);
    SAFE_RELEASE(oreImages);
}

void TileManager::Update()
{

}

void TileManager::Render(RECT cameraRect)
{
    glm::vec2 tileCurrFrame[1024];

    instancingShader->use();
    glBindVertexArray(tilesVAO->GetID());

    float vertices[] = {
        // positions       // texture coords
        -64 / 2, -64 / 2,  0.0f, 0.0f,
         64 / 2, -64 / 2,  1.0f, 0.0f,
         64 / 2,  64 / 2,  1.0f, 1.0f,
         64 / 2,  64 / 2,  1.0f, 1.0f,
        -64 / 2,  64 / 2,  0.0f, 1.0f,
        -64 / 2, -64 / 2,  0.0f, 0.0f,
    };
    tilesVAO->SetVBOData(0, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    instancingShader->setVec2("maxFrame", glm::vec2(64, 9));
    for (bigIt = mapChunks.begin(); bigIt != mapChunks.end(); bigIt++)
    {
        for (smallIt = bigIt->second.begin(); smallIt != bigIt->second.end(); smallIt++)
        {
            Chunk* currChunk = smallIt->second;
            if (!CheckRectCollision(currChunk->GetRect(), cameraRect))
                continue;

            instancingShader->setVec2("chunkCoord", currChunk->GetCoord());

            for (int y = 0; y < 32; y++)
            {
                for (int x = 0; x < 32; x++)
                {
                    tileCurrFrame[y * CHUNK_IN_TILE + x] = { x % 64, y % 4 };
                }
            }
            tilesVAO->SetVBOData(1, sizeof(glm::vec2) * 1024, &tileCurrFrame[0], GL_DYNAMIC_DRAW);

            for (int kind = 0; kind < int(Tile::KIND::END); kind++)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tileImages[kind].GetLpSourceTexture()->GetID());
                glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1024);    
            }
        }
    }
   
    float OreVertices[] = {
        // positions         // texture coords
        -128 / 2, -128 / 2,  0.0f, 0.0f,
         128 / 2, -128 / 2,  1.0f, 0.0f,
         128 / 2,  128 / 2,  1.0f, 1.0f,
         128 / 2,  128 / 2,  1.0f, 1.0f,
        -128 / 2,  128 / 2,  0.0f, 1.0f,
        -128 / 2, -128 / 2,  0.0f, 0.0f,
    };
    tilesVAO->SetVBOData(0, sizeof(OreVertices), OreVertices, GL_DYNAMIC_DRAW);

    instancingShader->setVec2("maxFrame", glm::vec2(8, 8));
    for (bigIt = mapChunks.begin(); bigIt != mapChunks.end(); bigIt++)
    {
        for (smallIt = bigIt->second.begin(); smallIt != bigIt->second.end(); smallIt++)
        {
            Chunk* currChunk = smallIt->second;
            if (!CheckRectCollision(currChunk->GetRect(), cameraRect))
                continue;

            instancingShader->setVec2("chunkCoord", currChunk->GetCoord());

            for (int y = 0; y < 32; y++)
            {
                for (int x = 0; x < 32; x++)
                {
                    Ore* ore = currChunk->GetLpTile(x, y)->GetLpOre();
                    tileCurrFrame[y * CHUNK_IN_TILE + x] = { ore->GetRandFrameX(), ore->AmountToImageFrameY() };
                }
            }
            tilesVAO->SetVBOData(1, sizeof(glm::vec2) * 1024, &tileCurrFrame[0], GL_DYNAMIC_DRAW);

            for (int kind = 0; kind < int(Ore::KIND::END); kind++)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, oreImages[kind].GetLpSourceTexture()->GetID());
                glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1024);
            }
        }
    }
    glBindVertexArray(0);
}
