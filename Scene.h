#pragma once
#include "GameNode.h"
class Scene : public GameNode
{
protected:
	int width, height;
	bool useBackBuffer;

public:
	bool IsUseBackBuffer() { return useBackBuffer; }
	void SetUseBackBuffer(bool b) { this->useBackBuffer = b; }

	Scene(int width, int height)
		: width(width)
		, height(height)
		, useBackBuffer(false)
	{};
	~Scene() {};
};

