#include "TileRenderer.h"
#include "Tile.h"
#include "GLImage.h"
#include "Shader.h"
#include "Texture.h"
#include "Ore.h"

HRESULT TileRenderer::Init()
{
    tileImages = new GLImage[int(Tile::KIND::END)]();
    tileImages[int(Tile::KIND::DIRT_1)].Init("Terrain/Dirt_1", 2048, 288, 64, 9);
    tileImages[int(Tile::KIND::DIRT_1)].SetZoom(2.0f);
    
    oreImages = new GLImage[int(Ore::KIND::END)]();
    oreImages[int(Ore::KIND::IRON_ORE)].Init("IronOre", 1024, 1024, 8, 8);
        
    tiles = new Tile[30 * 30]();
    for (int y = 0; y < 30; y++)
    {
        for (int x = 0; x < 30; x++)
        {
            tiles[y * 30 + x].Init(x, y);
        }
    }

	return S_OK;
}

void TileRenderer::Release()
{
    if (tiles)
    {
        for (int i = 0; i < 30 * 30; i++)
        {
            tiles[i].Release();
        }
    }
    SAFE_ARR_DELETE(tiles);
    SAFE_RELEASE(tileImages);
    SAFE_RELEASE(oreImages);
}

void TileRenderer::Update()
{
    for (int i = 0; i < 30 * 30; i++)
    {
        tiles[i].Update();
    }
}

void TileRenderer::Render(Shader* lpShader)
{
    float tileSizeX = tileImages[0].GetZoomedFrameWidth();
    float tilsSizeY = tileImages[0].GetZoomedFrameHeight();

    for (int kind = 0; kind < int(Tile::KIND::END); kind++)
    {
        tileImages[kind].MassiveRenderInit(lpShader);

        for (int y = 0; y < 30; y++)
        {
            for (int x = 0; x < 30; x++)
            {
                if (int(tiles[y * 30 + x].GetKind()) == kind)
                {
                    tileImages[kind].MassiveRender(lpShader, tileSizeX * x, tilsSizeY * y, x % 64, y % 4);
                }
            }
        }
        tileImages[kind].MassiveRenderRelease();
    }

    for (int kind = 0; kind < int(Ore::KIND::END); kind++)
    {
        oreImages[kind].MassiveRenderInit(lpShader);
        for (int y = 0; y < 30; y++)
        {
            for (int x = 0; x < 30; x++)
            {
                Ore* ore = tiles[y * 30 + x].GetLpOre();
                if (int(ore->GetKind()) == kind)
                {
                    oreImages[kind].MassiveRender(lpShader, tileSizeX * x, tilsSizeY * y, ore->GetRandFrameX(), ore->AmountToImageFrameY());
                }
            }
        }
        oreImages[kind].MassiveRenderRelease();
    }



}
