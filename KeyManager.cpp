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
		함수 호출 시점에 가상키(VK_...)가 어떤 상태인지 확인이 가능
		1. 0x0000 => 이전 프레임에 누른적이 없고 호출시점에도 눌려있지 않음
		2. 0x0001 => 이전 프레임에 누른적이 있고 호출시점에는 눌려있지 않음
		3. 0x8000 => 이전 프레임에 누른적이 없고 호출시점에는 눌려있는 상태
		4. 0x8001 => 이전 프레임에 누른적이 있고 호출시점에도 눌려있는 상태
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
	// 실습1. 위의 코드를 분석해서 완성해보자.
	if (GetAsyncKeyState(key) & 0x8000)	// return false인 경우
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
	// 실습2. 위의 코드를 분석해서 완성해보자.
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
