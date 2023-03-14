#pragma once
#include "../GameNode.h"

class Scene : public GameNode
{
protected:
	int width, height;
	bool useBackBuffer;

public:
	bool IsUseBackBuffer(); 

	void SetUseBackBuffer(bool b); 
	void SetWidthHeight(int _width, int _height); 

	int GetWidth();
	int GetHeight();

	Scene(int width, int height)
		: width(width)
		, height(height)
		, useBackBuffer(false)
	{};
	~Scene() {};
};

