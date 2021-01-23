#include "KeyManager.h"



HRESULT KeyManager::Init()
{
	for (int i = 0; i < KEY_MAX_COUNT; i++)
	{
		SetKeyDown(i, false);
		SetKeyUp(i, true);
	}

	return S_OK;
}

void KeyManager::Release()
{
	ReleaseSingleton();
}

bool KeyManager::IsOnceKeyDown(int key)
{
	if (!g_hWndFocus)
		return false;

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
		if (!keyDown[key])
		{
			keyDownTime[key] = g_time;
			SetKeyDown(key, true);
			return true;
		}
		else if (keyDownTime[key] == g_time)
		{
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
	if (!g_hWndFocus)
		return false;

	if (GetAsyncKeyState(key) & 0x8000)	// return false�� ���
	{
		SetKeyUp(key, false);
	}
	else	// return true
	{
		if (!GetKeyUp()[key])
		{
			keyUpTime[key] = g_time;
			SetKeyUp(key, true);
			return true;
		}
		else if (keyUpTime[key] == g_time) 
		{
			return true;
		}
	}

	return false;
}

bool KeyManager::IsStayKeyDown(int key)
{
	if (!g_hWndFocus)
		return false;

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
