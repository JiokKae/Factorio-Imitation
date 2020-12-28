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
        
    mapChunks[-1][-1] = new Chunk();
    mapChunks[-1][-1]->Init(-1, -1);
    mapChunks[0][0] = new Chunk();
    mapChunks[0][0]->Init(0, 0);
	return S_OK;
}

void TileRenderer::Release()
{

    SAFE_RELEASE(tileImages);
    SAFE_RELEASE(oreImages);
}

void TileRenderer::Update()
{

}

void TileRenderer::Render(Shader* lpShader)
{
    float tileSizeX = tileImages[0].GetZoomedFrameWidth();
    float tilsSizeY = tileImages[0].GetZoomedFrameHeight();

    map<int, map<int, Chunk*>>::iterator bigIt;
    map<int, Chunk*>::iterator smallIt;
    for (bigIt = mapChunks.begin(); bigIt != mapChunks.end(); bigIt++)
    {
        for (smallIt = bigIt->second.begin(); smallIt != bigIt->second.end(); smallIt++)
        {
            Chunk* currChunk = smallIt->second;
            for (int kind = 0; kind < int(Tile::KIND::END); kind++)
            {
                tileImages[kind].MassiveRenderInit(lpShader);

                for (int y = 0; y < CHUNK_IN_TILE; y++)
                {
                    for (int x = 0; x < CHUNK_IN_TILE; x++)
                    {
                        Tile* tile = currChunk->GetLpTile(x, y);
                        if (int(tile->GetKind()) == kind)
                        {
                            tileImages[kind].MassiveRender(lpShader, tileSizeX * tile->GetIndex().x + tileSizeX / 2, tilsSizeY * tile->GetIndex().y + tilsSizeY / 2, x % 64, y % 4);
                        }
                    }
                }
                tileImages[kind].MassiveRenderRelease();
            }

            for (int kind = 0; kind < int(Ore::KIND::END); kind++)
            {
                oreImages[kind].MassiveRenderInit(lpShader);
                for (int y = 0; y < CHUNK_IN_TILE; y++)
                {
                    for (int x = 0; x < CHUNK_IN_TILE; x++)
                    {
                        Tile* tile = currChunk->GetLpTile(x, y);
                        Ore* ore = currChunk->GetLpTile(x, y)->GetLpOre();
                        if (int(ore->GetKind()) == kind)
                        {
                            oreImages[kind].MassiveRender(lpShader, tileSizeX * tile->GetIndex().x + tileSizeX / 2, tilsSizeY * tile->GetIndex().y + tilsSizeY / 2, ore->GetRandFrameX(), ore->AmountToImageFrameY());
                        }
                    }
                }
                oreImages[kind].MassiveRenderRelease();
            }
        }
    }

}
