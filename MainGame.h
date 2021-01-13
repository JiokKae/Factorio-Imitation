#pragma once
#include "GameNode.h"
#include "framework.h"

class MainGame : public GameNode
{
private:
	HDC hdc;
	HGLRC hrc;

	bool isInit;
public:
	HRESULT Init();				// ��� ���� �ʱ�ȭ, �޸� �Ҵ�
	void Release();				// �޸� ����
	void Update();				// ������ ���� ���� ���� ���� (������ ����)
	void Render();				// ������ ���� ��� (�̹���, �ؽ�Ʈ ��)

	void ImageLoad();
	void SoundLoad();
	MainGame() {};

	LRESULT MainProc(HWND hWnd, UINT iMessage,
		WPARAM wParam, LPARAM lParam);
};
