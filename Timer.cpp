#include "Timer.h"

HRESULT Timer::Init()
{
	timeElapsed = 0.0f;
	currTime = 0;

	fpsTimeElapsed = 0.0f;
	fpsFrameCount = 0;

	if (QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency))
	{
		isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
		timeScale = 1.0f / periodFrequency;
	}
	else
	{
		isHardware = false;
		lastTime = timeGetTime();
		timeScale = 0.001f;
	}

	return S_OK;
}

void Timer::Tick()
{
	if (isHardware)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	}
	else
	{
		currTime = timeGetTime();
	}

	timeElapsed = (currTime - lastTime) * timeScale;

	fpsFrameCount++;
	fpsTimeElapsed += timeElapsed;
	if (fpsTimeElapsed >= 1.0f)
	{
		fpsTimeElapsed = 0.0f;
		fps = fpsFrameCount;
		fpsFrameCount = 0;
	}

	lastTime = currTime;
}
