#pragma once
#include "../GLFramework/UI/UI.h"

class TextUI : public UI
{
	string text;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(ShaderProgram* lpShader) override;

	void SetText(string str) { text = str; }
	string GetText() { return text; }
};

