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
	TextOut(hdc, 300, 40, szText,  static_cast<int>(strlen(szText)));
}

float TimerManager::GetTimeElapsed()
{
	return timer->GetTimeElapsed();
}

ULONG TimerManager::GetFPS()
{
	return timer->GetFPS();
}

float TimerManager::CheckTime()
{
	return timer->CheckTime();
}

void TimerManager::CheckRenderTime()
{
	renderTime = timer->CheckTime();
}

void TimerManager::CheckUpdateTime()
{
	updateTime = timer->CheckTime();
}

void TimerManager::SetIsSC_MOVE(bool isSC_MOVE)
{
	timer->SetIsSC_MOVE(isSC_MOVE);
}

void TimerManager::SetTargetFPS(ULONG targetFPS)
{
	timer->SetTargetFPS(targetFPS);
}
