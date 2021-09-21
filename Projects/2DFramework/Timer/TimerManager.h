#pragma once
#include "../framework.h"
#include "../Manager/Singleton.h"

class Timer;
class TimerManager : public Singleton<TimerManager>
{
private:
	Timer* timer;
	char szText[256];

public:
	float renderTime;
	float updateTime;

public:
	HRESULT Init();
	void Release();
	bool Update();
	void Render(HDC hdc);
	float GetTimeElapsed();
	ULONG GetFPS();
	float CheckTime();
	void CheckRenderTime();
	void CheckUpdateTime();

	void SetIsSC_MOVE(bool isSC_MOVE);
	void SetTargetFPS(ULONG targetFPS);
};

