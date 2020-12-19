#include "MainGame.h"
#include "Image.h"
#include "TitleScene.h"
#include "TileMapToolScene.h"
#include "TenCubeSpaceScene.h"
#include "LoadingScene1.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);

	SetWindowSize((1920 - WINSIZE_X)/2, (1080 - WINSIZE_Y) / 2,  WINSIZE_X, WINSIZE_Y);
	//ShowCursor(false);

	#pragma region Win OpenGL Init
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;
	memset(&pfd, 0, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER_DONTCARE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);

	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);
	glewInit();
	#pragma endregion
	
	TimerManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();

	// ¾À Ãß°¡
	SceneManager::GetSingleton()->AddScene("TitleScene", new TitleScene());
	SceneManager::GetSingleton()->AddScene("TileMapToolScene", new TileMapToolScene());
	SceneManager::GetSingleton()->AddScene("TenCubeSpaceScene", new TenCubeSpaceScene());
	SceneManager::GetSingleton()->AddLoadingScene("LoadingScene1", new LoadingScene1());

	SceneManager::GetSingleton()->ChangeScene("TileMapToolScene");

	TimerManager::GetSingleton()->SetTargetFPS(120);

	return S_OK;
}

void MainGame::Release()
{
	SceneManager::GetSingleton()->Release();
	TimerManager::GetSingleton()->Release();

	#pragma region Win OpenGL Release
	wglMakeCurrent(hdc, NULL);
	wglDeleteContext(hrc);
	#pragma endregion

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	SceneManager::GetSingleton()->Update();
	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	SceneManager::GetSingleton()->Render(hdc);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;

	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-1, 1, -1, 1, 1, -1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		break;

	case WM_MOUSEWHEEL:
		g_mousezDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
