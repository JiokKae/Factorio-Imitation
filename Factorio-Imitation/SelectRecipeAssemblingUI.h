#pragma once
#include "UI.h"
class DeactiveButtonUI;
class SelectRecipeAssemblingUI : public UI
{
	DeactiveButtonUI* deactiveButtonUI;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(Shader* lpShader) override;
};

