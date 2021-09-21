﻿#pragma once
#include "../2DFramework/GameNode.h"
#include "framework.h"

class MainGame : public GameNode
{
private:
	HDC hdc;
	HGLRC hrc;

	bool isInit;
public:
	HRESULT Init();				// 멤버 변수 초기화, 메모리 할당
	void Release();				// 메모리 해제
	void Update();				// 프레임 단위 게임 로직 실행 (데이터 변경)
	void Render();				// 프레임 단위 출력 (이미지, 텍스트 등)

	void ImageLoad();
	void SoundLoad();
	MainGame() 
		: hdc(NULL)
		, hrc(NULL)
		, isInit(false)
	{};

	LRESULT MainProc(HWND hWnd, UINT iMessage,
		WPARAM wParam, LPARAM lParam);
};
