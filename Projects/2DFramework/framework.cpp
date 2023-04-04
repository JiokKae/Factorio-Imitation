#include "framework.h"

const tagFRECT tagFRECT::operator+(const tagFRECT& rect) const
{
	tagFRECT ret;
	ret.left = left + rect.left;
	ret.top = top + rect.top;
	ret.right = right + rect.right;
	ret.bottom = bottom + rect.bottom;

	return ret;
}

TileInfoArgument::TileInfoArgument(TILE_INFO* lpTileInfo)
{
	this->tileInfo = lpTileInfo;
}

ChangeSceneArgument::ChangeSceneArgument(string sceneName, string loadingSceneName)
{
	this->sceneName = sceneName;
	this->loadingSceneName = loadingSceneName;
}
