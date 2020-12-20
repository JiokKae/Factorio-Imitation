#pragma once
#include "framework.h"
#include "Singleton.h"

class Texture;
class TextureManager : public Singleton<TextureManager>
{
private:
	map<string, Texture*> mapTextureDatas;
public:
	HRESULT Init();
	void Release();

	Texture* AddTexture(string strKey, char const* path, bool flip = true);

	void DeleteTexture(string strKey);
	Texture* FindTexture(string strKey);
};
