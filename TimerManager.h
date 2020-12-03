#pragma once
#include "framework.h"
#include "Singleton.h"

class Timer;
class TimerManager : public Singleton<TimerManager>
{
private:
	Timer* timer;

	char szText[256];

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	float GetTimeElapsed();
};

