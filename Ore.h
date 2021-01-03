#pragma once
#include "GameNode.h"

class Shader;
class Ore : public GameNode
{
public:
	enum class KIND {
		IRON_ORE,
		END
	};

private:
	KIND kind;
	int amount;
	int randFrameX;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader, float destX, float destY);

	KIND GetKind()		{ return kind; }
	int GetAmount()		{ return amount; }
	int GetRandFrameX() { return randFrameX; }
	int AmountToImageFrameY();

};

