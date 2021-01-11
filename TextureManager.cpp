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
			(it->second)->Release();
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

Texture* TextureManager::AddTexture(string strKey, char const* path, bool mipmap, bool flip, GLint filter)
{
	Texture* texture = nullptr;
	texture = FindTexture(strKey);
	if (texture)
	{
		return texture;
	}
	texture = new Texture();
	if (FAILED(texture->Init(path, mipmap, flip, filter)))
	{
		SAFE_RELEASE(texture);
		return nullptr;
	}

	mapTextureDatas.insert(make_pair(strKey, texture));

	return texture;
}

void TextureManager::DeleteTexture(string strKey)
{
	map<string, Texture*>::iterator it = mapTextureDatas.find(strKey);
	if (it != mapTextureDatas.end())
	{
		it->second->Release();
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
