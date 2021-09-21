#pragma once
#include "../GameNode.h"

class Scene : public GameNode
{
protected:
	int width, height;
	bool useBackBuffer;

public:
	bool IsUseBackBuffer() { return useBackBuffer; }

	void SetUseBackBuffer(bool b)			{ useBackBuffer = b; }
	void SetWidthHeight(int _width, int _height)	{ width = _width, height = _height; }
	
	int GetWidth()	{ return width; }
	int GetHeight() { return height; }

	Scene(int width, int height)
		: width(width)
		, height(height)
		, useBackBuffer(false)
	{};
	~Scene() {};
};

