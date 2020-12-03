#include "KeyManager.h"



HRESULT KeyManager::Init()
{
	for (int i = 0; i < KEY_MAX_COUNT; i++)
	{
		SetKeyDown(i, false);
		SetKeyUp(i, true);
		//keyUp[i] = true;
		//keyDown[i] = false;
	}

	return S_OK;
}

void KeyManager::Release()
{
	ReleaseSingleton();
}

bool KeyManager::IsOnceKeyDown(int key)
{
	// GetAsyncKeyState(VK_...)
	/*
		�Լ� ȣ�� ������ ����Ű(VK_...)�� � �������� Ȯ���� ����
		1. 0x0000 => ���� �����ӿ� �������� ���� ȣ��������� �������� ����
		2. 0x0001 => ���� �����ӿ� �������� �ְ� ȣ��������� �������� ����
		3. 0x8000 => ���� �����ӿ� �������� ���� ȣ��������� �����ִ� ����
		4. 0x8001 => ���� �����ӿ� �������� �ְ� ȣ��������� �����ִ� ����
	*/

	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!GetKeyDown()[key] /*keyDown[key] == false*/)
		{
			SetKeyDown(key, true) /*keyDown[key] = true*/;
			return true;
		}
	}
	else
	{
		SetKeyDown(key, false);
	}

	return false;
}

bool KeyManager::IsOnceKeyUp(int key)
{
	// �ǽ�1. ���� �ڵ带 �м��ؼ� �ϼ��غ���.
	if (GetAsyncKeyState(key) & 0x8000)	// return false�� ���
	{
		SetKeyUp(key, false);
	}
	else	// return true
	{
		if (!GetKeyUp()[key])
		{
			SetKeyUp(key, true);
			return true;
		}
	}

	return false;
}

bool KeyManager::IsStayKeyDown(int key)
{
	// �ǽ�2. ���� �ڵ带 �м��ؼ� �ϼ��غ���.
	if (GetAsyncKeyState(key) & 0x8000)
		return true;

	return false;
}

KeyManager::KeyManager()
{
}


KeyManager::~KeyManager()
{
}
