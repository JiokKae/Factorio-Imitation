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

	bool IsOnceKeyDown(int key);		// 키가 한번 눌렸는지 판단
	bool IsOnceKeyUp(int key);			// 키가 눌렸다가 떼졌는지 판단
	bool IsStayKeyDown(int key);		// 키가 눌려져 있는지 판단

	bitset<KEY_MAX_COUNT> GetKeyUp() { return keyUp; }
	bitset<KEY_MAX_COUNT> GetKeyDown() { return keyDown; }

private:
	void SetKeyUp(int key, bool state) { keyUp.set(key, state); }
	void SetKeyDown(int key, bool state) { keyDown.set(key, state); }

public:
	KeyManager();
	~KeyManager();
};

