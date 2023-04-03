#include "Chunk.h"
#include "Tile.h"
#include "Ore.h"

HRESULT Chunk::Init(int _x, int _y)
{
    coord = { _x, _y };
    tiles.reserve(CHUNK_IN_TILE * CHUNK_IN_TILE);
    for (int y = 0; y < CHUNK_IN_TILE; y++)
    {
        for (int x = 0; x < CHUNK_IN_TILE; x++)
        {
		tiles.push_back(new Tile(x + coord.x * CHUNK_IN_TILE, y + coord.y * CHUNK_IN_TILE));
        }
    }

	return S_OK;
}

void Chunk::Release()
{
    if (!tiles.empty())
    {
        for (int i = 0; i < CHUNK_IN_TILE * CHUNK_IN_TILE; i++)
        {
            SAFE_DELETE(tiles[i]);
        }
	tiles.clear();
    }
}

void Chunk::Update()
{
}

void Chunk::Render(ShaderProgram* /*lpShaderProgram*/)
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
    return tiles[y * CHUNK_IN_TILE + x];
}
