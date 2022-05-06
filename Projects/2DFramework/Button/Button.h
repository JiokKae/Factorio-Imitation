#pragma once
#include "../GameNode.h"

class Image;
class Button :public GameNode 
{
private:
	enum class STATE
	{
		NONE,
		UP,
		DOWN,
	};

private:
	STATE state;
	Image* img;
	RECT rc;
	POINT pos;
	
	POINT ptDownFrame;
	POINT ptUpFrame;

	// 버튼 기능 (함수 포인터)
	void (*buttonFunc)(void*);
	Argument_Kind kind;
	void* arg;
	
public:
	HRESULT Init(const char* imageName, int posX, int posY, POINT downFramePoint, POINT upFramePoint);
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(int mouseX, int mouseY, int windowSizeY);
	virtual void Render(HDC hdc) override;

	void SetButtonFunc(void (*_buttonFunc)(void*), Argument_Kind _kind = Argument_Kind::None, void* _arg = nullptr) {
		buttonFunc = _buttonFunc;
		kind = _kind;
		arg = _arg;
	}
	void ButtonFunc() { buttonFunc(arg); }

	Button();
	virtual ~Button() {}
};