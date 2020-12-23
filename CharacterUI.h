#pragma once
#include "UI.h"
#include "framework.h"

class CharacterUI : public UI
{

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};