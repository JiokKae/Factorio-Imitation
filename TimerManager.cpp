#include "TimerManager.h"
#include "Timer.h"

HRESULT TimerManager::Init()
{
	timer = new Timer();
	timer->Init();

	return S_OK;
}

void TimerManager::Release()
{
	delete timer;
	ReleaseSingleton();
}

bool TimerManager::Update()
{
	return timer->Tick();
}

void TimerManager::Render(HDC hdc)
{
	wsprintf(szText, "FPS : %d", timer->GetFPS());
	TextOut(hdc, WINSIZE_X - 300, 40, szText, strlen(szText));
}

float TimerManager::GetTimeElapsed()
{
	return timer->GetTimeElapsed();
}

ULONG TimerManager::GetFPS()
{
	return timer->GetFPS();
}

void TimerManager::SetIsSC_MOVE(bool isSC_MOVE)
{
	timer->SetIsSC_MOVE(isSC_MOVE);
}

void TimerManager::SetTargetFPS(ULONG targetFPS)
{
	timer->SetTargetFPS(targetFPS);
}
