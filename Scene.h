#pragma once
#include "GameNode.h"
class Scene : public GameNode
{
private:
	bool useBackBuffer;

public:
	bool IsUseBackBuffer() { return useBackBuffer; }
	void SetUseBackBuffer(bool b) { this->useBackBuffer = b; }
};

