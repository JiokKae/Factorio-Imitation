#pragma once
#include "UI.h"
class TextUI : public UI
{
	string text;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(Shader* lpShader) override;

	void SetText(string str) { text = str; }
	string GetText() { return text; }
};

