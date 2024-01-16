#include "TileManager.h"
#include "Tile.h"
#include "../GLFramework/Image/GLImage.h"
#include "../GLFramework/ShaderProgram/ShaderProgram.h"
#include "../GLFramework/Texture/Texture.h"
#include "Ore.h"
#include "Chunk.h"
#include "../GLFramework/VertexDataObject/VertexArrayObject.h"
#include "../GLFramework/VertexDataObject/VertexBufferObject.h"
#include "ItemId.h"

constexpr GLuint CURRFRAME_VBO_INDEX{ 1 };
constexpr GLuint OFFSET_VBO_INDEX{ 2 };

HRESULT TileManager::Init()
{
	tileImages[static_cast<int>(Tile::KIND::DIRT_1)] = new GLImage("Terrain/Dirt_1", 4096, 576, 64, 9);

	mapOreImages[COAL] = new GLImage("Entity/Coal", 8, 8);
	mapOreImages[IRON_ORE] = new GLImage("Entity/IronOre", 8, 8);
	mapOreImages[COPPER_ORE] = new GLImage("Entity/CopperOre", 8, 8);
	mapOreImages[STONE] = new GLImage("Entity/Stone", 8, 8);

    // 14 * 14 청크 생성
    for (int y = -7; y < 7; y++)
    {
        for (int x = -7; x < 7; x++)
        {
            mapChunks[y][x] = new Chunk(x, y);
        }
    }

    // 인스턴싱 쉐이더 생성 및 초기화
    instancingShader = new ShaderProgram("shader/InstancingVertexShader.glsl", "shader/StandardFragmentShader.glsl");
    instancingShader->use();
    instancingShader->setInt("material.diffuse", 0);
    instancingShader->setVec3("material.diffuseColor", glm::vec3(1.0f, 1.0f, 1.0f));
    instancingShader->setFloat("alpha", 1.0f);
    instancingShader->setFloat("vertexScale", 1.0f);
    instancingShader->setMat4("model", glm::mat4());
    
    // 아래의 VBO들을 담는 Vertex Array Object를 생성한다.
    tilesVAO = new VAO();

    // tileQuadVBO Init (사각형 버텍스들을 저장하는 VBO)
    constexpr float vertices[] = {
        // positions       // texture coords
        -64 / 2, -64 / 2,  0.0f, 0.0f,
         64 / 2, -64 / 2,  1.0f, 0.0f,
         64 / 2,  64 / 2,  1.0f, 1.0f,
         64 / 2,  64 / 2,  1.0f, 1.0f,
        -64 / 2,  64 / 2,  0.0f, 1.0f,
        -64 / 2, -64 / 2,  0.0f, 0.0f,
    };
    VBO* tileQuadVBO = new VBO(sizeof(float) * 4 * 6, vertices, GL_STATIC_DRAW);   // vec4(vec2 pos, vec2 texCoord) * 6
    tilesVAO->AddVBO(0, tileQuadVBO, 4);

    // currFrameVBO Init (각 인스턴스의 현재 프레임을 저장하는 VBO)
    VBO* currFrameVBO = new VBO(sizeof(glm::vec2) * 1024, NULL, GL_DYNAMIC_DRAW); // vec2(currFrame.x, currFrame.y) * 1024
    tilesVAO->AddVBO(CURRFRAME_VBO_INDEX, currFrameVBO, 2);
    tilesVAO->SetDivisor(CURRFRAME_VBO_INDEX, 1);
    
    // offsetVBO Init (각 인스턴스의 위치 오프셋을 저장하는 VBO)
    VBO* offsetVBO = new VBO(sizeof(glm::vec2) * 1024, NULL, GL_DYNAMIC_DRAW); // vec2(offset.x, offset.y) * 1024
    tilesVAO->AddVBO(OFFSET_VBO_INDEX, offsetVBO, 2);
    tilesVAO->SetDivisor(OFFSET_VBO_INDEX, 1);

    tileCurrFrame = new glm::vec2[TILE_IN_CHUNK_ROW * TILE_IN_CHUNK_ROW];
    tileOffset = new glm::vec2[TILE_IN_CHUNK_ROW * TILE_IN_CHUNK_ROW];
    for (int y = 0; y < TILE_IN_CHUNK_ROW; y++)
    {
        for (int x = 0; x < TILE_IN_CHUNK_ROW; x++)
        {
            tileCurrFrame[y * TILE_IN_CHUNK_ROW + x] = { x % 64, y % 4 };
            /*
            Tile* tile = currChunk->GetLpTile(x, y);
            if(tile->GetLpSturcture() != nullptr)
                tileCurrFrame[y * CHUNK_IN_TILE + x] = { x % 64, y % 4 };
            else
                tileCurrFrame[y * CHUNK_IN_TILE + x] = { 100, 100 };
            */
            tileOffset[y * TILE_IN_CHUNK_ROW + x] = { x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2 };
        }
    }
	return S_OK;
}

void TileManager::Release()
{
    SAFE_DELETE(tilesVAO);
    SAFE_DELETE(instancingShader);

	for (auto& image : tileImages)
	{
		SAFE_DELETE(image.second);
	}
  
	for (auto& image : mapOreImages)
	{
		SAFE_DELETE(image.second);
	}

    SAFE_ARR_DELETE(tileOffset);
    SAFE_ARR_DELETE(tileCurrFrame);
    for (auto& chunks : mapChunks)
    {
	    for (auto& chunk : chunks.second)
	    {
		    SAFE_DELETE(chunk.second);
	    }
    }
    
    ReleaseSingleton();
}

void TileManager::Update()
{

}

void TileManager::Render(const RECT& cameraRect)
{
	vecChunkInScreen.clear();
	for (auto& [y, xChunks] : mapChunks)
	{
		for (auto& [x, chunk] : xChunks)
		{
			if (chunk && CheckRectCollision(chunk->GetRect(), cameraRect))
			{
				vecChunkInScreen.push_back(chunk);
			}
		}
	}

	instancingShader->use();
	glBindVertexArray(tilesVAO->GetID());
	instancingShader->setFloat("vertexScale", 1.0f);
	instancingShader->setVec2("maxFrame", glm::vec2(64, 9));

	for (Chunk* chunk : vecChunkInScreen)
	{
		instancingShader->setVec2("offset", chunk->GetCoord() * 2048);

		tilesVAO->SetVBOData(CURRFRAME_VBO_INDEX, sizeof(glm::vec2) * 1024, &tileCurrFrame[0], GL_DYNAMIC_DRAW);
		tilesVAO->SetVBOData(OFFSET_VBO_INDEX, sizeof(glm::vec2) * 1024, &tileOffset[0], GL_DYNAMIC_DRAW);
		for (int kind = 0; kind < int(Tile::KIND::END); kind++)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tileImages[kind]->GetLpSourceTexture()->GetID());
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1024);
		}
	}

	DebugHelper::GetSingleton()->SetFloat("2.1. tileRender", TimerManager::GetSingleton()->CheckTime());

	instancingShader->setFloat("vertexScale", 2.0f);
	instancingShader->setVec2("maxFrame", glm::vec2(8, 8));
	for (Chunk* chunk : vecChunkInScreen)
	{
		instancingShader->setVec2("offset", chunk->GetCoord() * 2048);

		for (auto& [itemId, oreImage] : mapOreImages)
		{
			int count = 0;

			for (int y = 0; y < TILE_IN_CHUNK_ROW; y++)
			{
				for (int x = 0; x < TILE_IN_CHUNK_ROW; x++)
				{
					Ore* ore = chunk->GetLpTile(x, y)->GetLpOre();
					if (ore->GetItemEnum() == itemId && ore->GetAmount() > 0)
					{
						oreCurrFrame[count] = { ore->GetRandFrameX(), ore->GetFrameY() };
						oreOffset[count] = { x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2 };
						count++;
					}
				}
			}
			tilesVAO->SetVBOData(CURRFRAME_VBO_INDEX, sizeof(glm::vec2) * count, &oreCurrFrame[0], GL_DYNAMIC_DRAW);
			tilesVAO->SetVBOData(OFFSET_VBO_INDEX, sizeof(glm::vec2) * count, &oreOffset[0], GL_DYNAMIC_DRAW);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, oreImage->GetLpSourceTexture()->GetID());
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
        indexInChunk.x = coordX % TILE_IN_CHUNK_ROW;
        indexInChunk.y = coordY % TILE_IN_CHUNK_ROW;

        if (indexInChunk.x < 0)
            indexInChunk.x += TILE_IN_CHUNK_ROW;
        if (indexInChunk.y < 0)
            indexInChunk.y += TILE_IN_CHUNK_ROW;

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

Tile* TileManager::GetLpCursorTile()
{
    return GetLpTile(g_cursorCoord.x, g_cursorCoord.y);
}

Structure* TileManager::GetLpCursorStructure()
{
	return GetLpCursorTile() ? GetLpCursorTile()->GetLpSturcture() : nullptr;
}

Chunk* TileManager::GetLpChunk(int x, int y)
{
    return mapChunks[y][x];
}
