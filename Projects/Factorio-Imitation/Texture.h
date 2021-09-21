#pragma once
#include "framework.h"
// CC

class Texture
{
	unsigned int ID;
	int width;
	int height;
	int nrComponents;
	GLenum format;

public:
	HRESULT Init(char const* path, bool mipmap = true, bool flip = true, GLint filter = GL_NEAREST);
	void Release();

	unsigned int GetID()	{ return ID; }
	int GetWidth()			{ return width; }
	int GetHeight()			{ return height; }

	Texture() {};
	~Texture() {};
};

