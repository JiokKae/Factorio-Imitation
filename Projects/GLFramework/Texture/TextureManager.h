#pragma once
#include "../framework.h"

class Texture;
class TextureManager : public Singleton<TextureManager>
{
private:
	map<string, Texture*> mapTextureDatas;
	GLint GetFilterInt(const std::string& filter);

public:
	HRESULT Init();
	void Release();

	Texture* AddTexture(string strKey, char const* path, bool mipmap, bool flip, const std::string& filter);
	Texture* AddTexture(string strKey, char const* path, bool mipmap = true, bool flip = true, GLint filter = GL_LINEAR);

	void DeleteTexture(string strKey);
	Texture* FindTexture(string strKey);
};
