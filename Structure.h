#pragma once
#include "GameNode.h"

class Shader;
class Structure : public GameNode
{
public:
	enum class DIRECTION {
		NORTH,
		EAST,
		WEST,
		SOUTH,
		END
	};

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};

