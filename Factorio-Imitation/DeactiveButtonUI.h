#pragma once
#include "UI.h"
class DeactiveButtonUI : public UI
{
	bool onMouse;
	bool isMouseDown;
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);
};

