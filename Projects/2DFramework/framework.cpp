#include "framework.h"

tagFRECT tagFRECT::operator+(const tagFRECT& rect) const
{
	tagFRECT ret{};
	ret.left = left + rect.left;
	ret.top = top + rect.top;
	ret.right = right + rect.right;
	ret.bottom = bottom + rect.bottom;

	return ret;
}

TileInfoArgument::TileInfoArgument(TILE_INFO* lpTileInfo)
	: tileInfo{ lpTileInfo }
{
}

ChangeSceneArgument::ChangeSceneArgument(const std::string& sceneName, const std::string& loadingSceneName)
	: sceneName{ sceneName }
	, loadingSceneName{ loadingSceneName }
{
}
