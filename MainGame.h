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



	int width, height, nrChannels;
	unsigned char* data;
	unsigned int texture;

public:
	HRESULT Init();				// ��� ���� �ʱ�ȭ, �޸� �Ҵ�
	void Release();				// �޸� ����
	void Update();				// ������ ���� ���� ���� ���� (������ ����)
	void Render();				// ������ ���� ��� (�̹���, �ؽ�Ʈ ��)

	LRESULT MainProc(HWND hWnd, UINT iMessage,
		WPARAM wParam, LPARAM lParam);
};

