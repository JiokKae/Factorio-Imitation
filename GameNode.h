#pragma once
#include "framework.h"

/*
	������ ���� �⺻���� ������ �Ǵ� Ŭ����
*/
class GameNode
{
public:
	virtual HRESULT Init();				// ��� ���� �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();				// �޸� ����
	virtual void Update();				// ������ ���� ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc);		// ������ ���� ��� (�̹���, �ؽ�Ʈ ��)

	GameNode();
	virtual ~GameNode();
};
