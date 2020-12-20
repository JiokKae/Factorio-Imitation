#pragma once
#include "framework.h"

class Texture
{
	unsigned int ID;
	int width;
	int height;
	int nrComponents;
	GLenum format;

public:
	HRESULT Init(char const* path, bool flip = true);
	void Release();

	unsigned int GetID() { return ID; }

	Texture() {};
	~Texture() {};
};

