#include "Chunk.h"
#include "Tile.h"
#include "Ore.h"
HRESULT Chunk::Init(int x, int y)
{
    coord = { x, y };
    tiles = new Tile[CHUNK_IN_TILE * CHUNK_IN_TILE]();
    for (int y = 0; y < CHUNK_IN_TILE; y++)
    {
        for (int x = 0; x < CHUNK_IN_TILE; x++)
        {
            tiles[y * CHUNK_IN_TILE + x].Init(x + coord.x * CHUNK_IN_TILE, y + coord.y * CHUNK_IN_TILE);
        }
    }

	return S_OK;
}

void Chunk::Release()
{
    if (tiles)
    {
        for (int i = 0; i < CHUNK_IN_TILE * CHUNK_IN_TILE; i++)
        {
            tiles[i].Release();
        }
    }
    SAFE_ARR_DELETE(tiles);
}

void Chunk::Update()
{
    for (int i = 0; i < CHUNK_IN_TILE * CHUNK_IN_TILE; i++)
    {
        tiles[i].Update();
    }
}

void Chunk::Render(Shader* lpShader)
{

}

RECT Chunk::GetRect()
{
    RECT rect;

    rect.left = coord.x * 2048;
    rect.right = (coord.x + 1) * 2048;
    rect.top = (coord.y + 1) * 2048;
    rect.bottom = coord.y * 2048;

    return rect;
}

Tile* Chunk::GetLpTile(int x, int y)
{
    return &tiles[y * CHUNK_IN_TILE + x];
}
