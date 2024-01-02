#pragma once
#include "../framework.h"
#include <string>

class Texture;
class TextureManager : public Singleton<TextureManager>
{
public:
	HRESULT Init();
	void Release();

	void AddTexture(const std::string& key, char const* path, bool mipmap, bool flip, const std::string& filterName);
	void AddTexture(const std::string& key, char const* path, bool mipmap = true, bool flip = true, GLint filter = GL_LINEAR);

	Texture* FindTexture(const std::string& key);

private:
	GLint GetFilterInt(const std::string& filterName) const;

	std::map<std::string, Texture*> mapTextureData;
};
