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
	bool Update();
	void Render(HDC hdc);
	float GetTimeElapsed();
	ULONG GetFPS();

	void SetIsSC_MOVE(bool isSC_MOVE);
	void SetTargetFPS(ULONG targetFPS);
};

