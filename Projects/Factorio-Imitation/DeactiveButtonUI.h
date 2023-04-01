#pragma once
#include "framework.h"
#include "GLFramework/UI/UI.h"

class DeactiveButtonUI : public UI
{
	bool onMouse;
	bool isMouseDown;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(ShaderProgram* lpShader);
};

