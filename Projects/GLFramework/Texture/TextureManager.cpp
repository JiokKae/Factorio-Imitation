#include "TextureManager.h"
#include "Texture.h"

HRESULT TextureManager::Init()
{
	return S_OK;
}

void TextureManager::Release()
{
	for (auto& textureData : mapTextureData)
	{
		if (textureData.second)
		{
			delete textureData.second;
			textureData.second = nullptr;
		}
	}
	mapTextureData.clear();

	ReleaseSingleton();
}

void TextureManager::AddTexture(const std::string& key, char const* path, bool mipmap, bool flip, const std::string& filterName)
{
	AddTexture(key, path, mipmap, flip, GetFilterInt(filterName));
}

void TextureManager::AddTexture(const std::string& key, char const* path, bool mipmap /*= true*/, bool flip /*= true*/, GLint filter /*= GL_LINEAR*/)
{
	Texture* texture{ FindTexture(key) };
	if (texture)
	{
		return;
	}

	try
	{
		texture = new Texture(path, mipmap, flip, filter);
	}
	catch (Texture::LoadImageException e)
	{
		return;
	}

	mapTextureData.emplace(key, texture);
}

Texture* TextureManager::FindTexture(const std::string& key)
{
	auto iter = mapTextureData.find(key);
	if (iter != mapTextureData.end())
	{
		return iter->second;
	}
	return nullptr;
}

GLint TextureManager::GetFilterInt(const std::string& filterName) const
{
	const static std::map<std::string, GLint> filters{
		{"GL_NEAREST", GL_NEAREST},
		{"GL_LINEAR", GL_LINEAR},
		{"GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST},
		{"GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST},
		{"GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR},
		{"GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR},
	};

	auto itr = filters.find(filterName);
	return itr != filters.end() ? itr->second : GL_LINEAR;
}
