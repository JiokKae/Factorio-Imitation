#include "TextureManager.h"
#include "Texture.h"

HRESULT TextureManager::Init()
{
	return S_OK;
}

void TextureManager::Release()
{
	map<string, Texture*>::iterator it;
	for (it = mapTextureDatas.begin(); it != mapTextureDatas.end(); )
	{
		if (it->second)
		{
			delete (it->second);
			it = mapTextureDatas.erase(it);
		}
		else
		{
			it++;
		}
	}
	mapTextureDatas.clear();

	ReleaseSingleton();
}

GLint TextureManager::GetFilterInt(const std::string& filter)
{
	const static std::map<std::string, GLint> filters{ 
		{"GL_NEAREST", GL_NEAREST},
		{"GL_LINEAR", GL_LINEAR},
		{"GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST},
		{"GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST},
		{"GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR},
		{"GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR},
	};
	
	auto itr = filters.find(filter);
	return itr != filters.end() ? itr->second : GL_LINEAR;
}

Texture* TextureManager::AddTexture(string strKey, char const* path, bool mipmap, bool flip, const std::string& filter)
{
	return AddTexture(strKey, path, mipmap, flip, GetFilterInt(filter));
}

Texture* TextureManager::AddTexture(string strKey, char const* path, bool mipmap, bool flip, GLint filter)
{
	Texture* texture{ FindTexture(strKey) };
	if (texture)
	{
		return texture;
	}

	try
	{
		texture = new Texture(path, mipmap, flip, filter);
	}
	catch (Texture::LoadImageException e)
	{
		return nullptr;
	}

	mapTextureDatas.emplace(strKey, texture);

	return texture;
}

void TextureManager::DeleteTexture(string strKey)
{
	map<string, Texture*>::iterator it = mapTextureDatas.find(strKey);
	if (it != mapTextureDatas.end())
	{
		delete it->second;

		mapTextureDatas.erase(it);
	}
}

Texture* TextureManager::FindTexture(string strKey)
{
	map<string, Texture*>::iterator it = mapTextureDatas.find(strKey);
	if (it != mapTextureDatas.end())
	{
		return it->second;
	}

	return nullptr;
}
