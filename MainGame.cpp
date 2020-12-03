#include "MainGame.h"
#include "Image.h"
#include "TitleScene.h"
#include "TileMapToolScene.h"
#include "LoadingScene1.h"

HRESULT MainGame::Init()
{
	TimerManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();

	hdc = GetDC(g_hWnd);

	backBuffer = new Image();
	backBuffer->Init(WINSIZE_X, WINSIZE_Y);

	// 씬 추가
	SceneManager::GetSingleton()->AddScene("TitleScene", new TitleScene());
	SceneManager::GetSingleton()->AddScene("TileMapToolScene", new TileMapToolScene());
	SceneManager::GetSingleton()->AddLoadingScene("LoadingScene1", new LoadingScene1());

	SceneManager::GetSingleton()->ChangeScene("TitleScene");

	return S_OK;
}

void MainGame::Release()
{
	SAFE_RELEASE(backBuffer);

	SceneManager::GetSingleton()->Release();
	TimerManager::GetSingleton()->Release();

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	SceneManager::GetSingleton()->Update();

	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	HDC backDC = backBuffer->GetMemDC();

	SceneManager::GetSingleton()->Render(backDC);
	
	char szText[128];

	wsprintf(szText, "X : %d, Y : %d", g_ptMouse.x, g_ptMouse.y);
	TextOut(backDC, 10, 5, szText, strlen(szText));

	wsprintf(szText, "g_time : %d", (int)g_time);
	TextOut(backDC, WINSIZE_X - 300, 0, szText, strlen(szText));

	TimerManager::GetSingleton()->Render(backDC);

	// 백버퍼 복사(출력)
	backBuffer->Render(hdc, 0, 0, WINSIZE_X, WINSIZE_Y);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
