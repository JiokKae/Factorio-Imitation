#include "TileRenderer.h"
#include "Tile.h"
#include "GLImage.h"
#include "Shader.h"
#include "Texture.h"
#include "Ore.h"
#include "Chunk.h"

HRESULT TileRenderer::Init()
{
    tileImages = new GLImage[int(Tile::KIND::END)]();
    tileImages[int(Tile::KIND::DIRT_1)].Init("Terrain/Dirt_1", 4096, 576, 64, 9);
    
    oreImages = new GLImage[int(Ore::KIND::END)]();
    oreImages[int(Ore::KIND::IRON_ORE)].Init("IronOre", 1024, 1024, 8, 8);
        
    for (int y = -7; y < 7; y++)
    {
        for (int x = -7; x < 7; x++)
        {
            mapChunks[y][x] = new Chunk();
            mapChunks[y][x]->Init(x, y);
        }
    }

    glm::vec2 tileOffset[1024];
    glm::vec2 tileCurrFrame[1024];
    int index = 0;
    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 32; x++)
        {
            tileCurrFrame[index] = {x % 64, y % 4};
            tileOffset[index++] = { x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2 };
        }
    }

    instancingShader = new Shader("InstancingVertexShader.glsl", "StandardFragmentShader.glsl");
    instancingShader->use();
    instancingShader->setVec2("currFrame", { 0 , 0 });
    instancingShader->setInt("material.diffuse", 0);
    instancingShader->setFloat("alpha", 1.0f);
    
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    // world transformation
    glm::mat4 planeModel;
    planeModel = glm::translate(planeModel, position);
    planeModel = glm::scale(planeModel, glm::vec3(1.0f));
    instancingShader->setMat4("model", planeModel);
    
    glGenBuffers(1, &offsetVBO);
    glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 1024, &tileOffset[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &currFrameVBO);
    glBindBuffer(GL_ARRAY_BUFFER, currFrameVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 1024, &tileCurrFrame[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    float vertices[] = {
        // positions       // texture coords
        -64 / 2, -64 / 2,  0.0f, 0.0f,
         64 / 2, -64 / 2,  1.0f, 0.0f,
         64 / 2,  64 / 2,  1.0f, 1.0f,
         64 / 2,  64 / 2,  1.0f, 1.0f,
        -64 / 2,  64 / 2,  0.0f, 1.0f,
        -64 / 2, -64 / 2,  0.0f, 0.0f,
    };
    
    glGenVertexArrays(1, &tilesVAO);
    glGenBuffers(1, &tileQuadVBO);
    glBindVertexArray(tilesVAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, tileQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // also set instance data
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, currFrameVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, offsetVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

	return S_OK;
}

void TileRenderer::Release()
{
    SAFE_DELETE(instancingShader);
    SAFE_RELEASE(tileImages);
    SAFE_RELEASE(oreImages);
}

void TileRenderer::Update()
{

}

void TileRenderer::Render(Shader* lpShader)
{
    instancingShader->use();
    glBindVertexArray(tilesVAO);
    float vertices[] = {
        // positions       // texture coords
        -64 / 2, -64 / 2,  0.0f, 0.0f,
         64 / 2, -64 / 2,  1.0f, 0.0f,
         64 / 2,  64 / 2,  1.0f, 1.0f,
         64 / 2,  64 / 2,  1.0f, 1.0f,
        -64 / 2,  64 / 2,  0.0f, 1.0f,
        -64 / 2, -64 / 2,  0.0f, 0.0f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, tileQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    instancingShader->setVec2("maxFrame", glm::vec2(64, 9));
    for (bigIt = mapChunks.begin(); bigIt != mapChunks.end(); bigIt++)
    {
        for (smallIt = bigIt->second.begin(); smallIt != bigIt->second.end(); smallIt++)
        {
            Chunk* currChunk = smallIt->second;
            instancingShader->setVec2("chunkCoord", currChunk->GetIndex());

            glm::vec2 tileCurrFrame[1024];
            int index = 0;
            for (int y = 0; y < 32; y++)
            {
                for (int x = 0; x < 32; x++)
                {
                    tileCurrFrame[index++] = { x % 64, y % 4 };
                }
            }
            glBindBuffer(GL_ARRAY_BUFFER, currFrameVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 1024, &tileCurrFrame[0], GL_DYNAMIC_DRAW);


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
    glBindBuffer(GL_ARRAY_BUFFER, tileQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(OreVertices), OreVertices, GL_STATIC_DRAW);

    instancingShader->setVec2("maxFrame", glm::vec2(8, 8));
    for (bigIt = mapChunks.begin(); bigIt != mapChunks.end(); bigIt++)
    {
        for (smallIt = bigIt->second.begin(); smallIt != bigIt->second.end(); smallIt++)
        {
            Chunk* currChunk = smallIt->second;
            instancingShader->setVec2("chunkCoord", currChunk->GetIndex());
            
            glm::vec2 tileCurrFrame[1024];
            int index = 0;
            for (int y = 0; y < 32; y++)
            {
                for (int x = 0; x < 32; x++)
                {
                    Ore* ore = currChunk->GetLpTile(x, y)->GetLpOre();
                    tileCurrFrame[index++] = { ore->GetRandFrameX(), ore->AmountToImageFrameY() };
                }
            }
            glBindBuffer(GL_ARRAY_BUFFER, currFrameVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 1024, &tileCurrFrame[0], GL_DYNAMIC_DRAW);

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
