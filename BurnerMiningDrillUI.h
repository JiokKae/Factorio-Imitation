#pragma once
#include "UI.h"
class BurnerMiningDrillUI : public UI
{
public:
	virtual HRESULT Init() override;
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};

