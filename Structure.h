#pragma once
#include "Entity.h"

class Shader;
class Structure : public Entity
{
public:
	enum DIRECTION{
		NORTH,
		EAST,
		SOUTH,
		WEST,
		END
	};

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};

