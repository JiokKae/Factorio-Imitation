#pragma once
#include <glew/glew.h>
#include <string>

class Texture final
{
public:
	class LoadImageException : public std::exception
	{
	public:
		LoadImageException(const std::string& path);
		const char* what() const noexcept override;
	private:
		std::string message;
	};

	Texture(const std::string& path, bool mipmap = true, bool flip = true, GLint filter = GL_NEAREST);
	~Texture();

	unsigned int GetID() const;
	int GetWidth() const;
	int GetHeight() const;

private:
	unsigned int ID;
	int width;
	int height;

	GLint MinFilter(GLint filter, bool mipmap);
	GLenum Format(int nrComponents);
};
