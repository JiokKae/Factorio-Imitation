#pragma once
#include "GameNode.h"
#include "framework.h"
class Image;
class MainGame : public GameNode
{
private:
	HDC hdc;
	HDC backDC;
	HGLRC hrc;
	Image* backBuffer;

public:
	HRESULT Init();				// ��� ���� �ʱ�ȭ, �޸� �Ҵ�
	void Release();				// �޸� ����
	void Update();				// ������ ���� ���� ���� ���� (������ ����)
	void Render();				// ������ ���� ��� (�̹���, �ؽ�Ʈ ��)

	LRESULT MainProc(HWND hWnd, UINT iMessage,
		WPARAM wParam, LPARAM lParam);
};

