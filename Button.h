#pragma once
#include "GameNode.h"

enum BUTTON_STATE
{
	NONE,
	UP,
	DOWN
};

class Image;
class Button :public GameNode 
{
private:
	BUTTON_STATE state;
	Image* img;
	RECT rc;
	POINT pos;
	
	POINT ptDownFrame;
	POINT ptUpFrame;

	// 버튼 기능 (함수 포인터)
	void (*buttonFunc)(void*);
	void* arg;
	
public:
	HRESULT Init(const char* imageName, int posX, int posY, POINT downFramePoint, POINT upFramePoint);
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void SetButtonFunc(void (*buttonFunc)(void*), void* arg) { 
		this->buttonFunc = buttonFunc; 
		this->arg = arg; 
	}
	void ButtonFunc() { buttonFunc(arg); }

	Button();
	virtual ~Button() {}
};