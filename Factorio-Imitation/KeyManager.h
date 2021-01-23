#pragma once
#include "framework.h"
#include "Singleton.h"
#include <bitset>

using namespace std;

#define KEY_MAX_COUNT	256

class KeyManager : public Singleton<KeyManager>
{
private:
	bitset<KEY_MAX_COUNT>	keyUp;
	bitset<KEY_MAX_COUNT>	keyDown;
	float keyDownTime[KEY_MAX_COUNT];
	float keyUpTime[KEY_MAX_COUNT];
public:
	HRESULT Init();
	void Release();

	bool IsOnceKeyDown(int key);		// Ű�� �ѹ� ���ȴ��� �Ǵ�
	bool IsOnceKeyUp(int key);			// Ű�� ���ȴٰ� �������� �Ǵ�
	bool IsStayKeyDown(int key);		// Ű�� ������ �ִ��� �Ǵ�

	bitset<KEY_MAX_COUNT> GetKeyUp() { return keyUp; }
	bitset<KEY_MAX_COUNT> GetKeyDown() { return keyDown; }

private:
	void SetKeyUp(int key, bool state) { keyUp.set(key, state); }
	void SetKeyDown(int key, bool state) { keyDown.set(key, state); }

public:
	KeyManager();
	~KeyManager();
};

