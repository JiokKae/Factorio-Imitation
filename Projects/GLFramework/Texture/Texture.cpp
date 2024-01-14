#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../../Librarys/stb_image/stb_image.h"

Texture::Texture(const std::string& path, bool mipmap /*= true*/, bool flip /*= true*/, GLint filter /*= GL_NEAREST*/)
{
	stbi_set_flip_vertically_on_load(flip);

	int nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, STBI_rgb_alpha);
	if (data == nullptr)
	{
		throw LoadImageException(path);
	}

	if (nrComponents != 2)
	{
		data = stbi_load(path.c_str(), &width, &height, &nrComponents, nrComponents);
	}

	glGenTextures(1, &ID);

	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, Format(nrComponents), width, height, 0, Format(nrComponents), GL_UNSIGNED_BYTE, data);

	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter(filter, mipmap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

unsigned int Texture::GetID() const
{
	return ID;
}

int Texture::GetWidth() const
{
	return width;
}

int Texture::GetHeight() const
{
	return height;
}

GLint Texture::MinFilter(GLint filter, bool mipmap)
{
	if (mipmap == false)
	{
		return filter;
	}

	if (filter == GL_NEAREST)
	{
		return GL_NEAREST_MIPMAP_NEAREST; // GL_NEAREST_MIPMAP_LINEAR
	}

	return GL_LINEAR_MIPMAP_LINEAR;
}

GLenum Texture::Format(int nrComponents)
{
	switch (nrComponents)
	{
	case 1:
		return GL_RED;
	case 2:
		return GL_RGBA;
	case 3:
		return GL_RGB;
	case 4:
		return GL_RGBA;
	default:
		return GLenum{};
	}
}

Texture::LoadImageException::LoadImageException(const std::string& path)
	: message(std::string("Texture failed to load at path: ") + path)
{
}

const char* Texture::LoadImageException::what() const noexcept
{
	return message.c_str();
}
