#pragma once
#include "UI.h"
class BurnerMiningDrillUI : public UI
{
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};

