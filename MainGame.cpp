#include "MainGame.h"
#include "Image.h"
#include "TitleScene.h"
#include "TileMapToolScene.h"
#include "TenCubeSpaceScene.h"
#include "LightingScene.h"
#include "LoadingScene1.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);

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
	
	// Singleton Init
	TimerManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();
	SoundManager::GetSingleton()->Init();

	// Add Sound
	SoundManager::GetSingleton()->AddSound("DarkWaltz", "Sound/Dark Waltz.mp3", true, false);

	// Add Scene
	SceneManager::GetSingleton()->AddScene("TitleScene", new TitleScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("TileMapToolScene", new TileMapToolScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("TenCubeSpaceScene", new TenCubeSpaceScene(900, 900));
	SceneManager::GetSingleton()->AddScene("LightingScene", new LightingScene(900, 900));
	
	SceneManager::GetSingleton()->AddLoadingScene("LoadingScene1", new LoadingScene1(1600, 900));

	ImageLoad();

	// Change StartScene
	SceneManager::GetSingleton()->ChangeScene("TenCubeSpaceScene");

	TimerManager::GetSingleton()->SetTargetFPS(120);

	return S_OK;
}

void MainGame::Release()
{
	// Singleton Release
	SceneManager::GetSingleton()->Release();
	TimerManager::GetSingleton()->Release();
	SoundManager::GetSingleton()->Release();

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

void MainGame::ImageLoad()
{
	ImageManager::GetSingleton()->AddImage("TitleSceneImage", "Image/bin.bmp", WINSIZE_TITLE_X, WINSIZE_TITLE_Y);
	ImageManager::GetSingleton()->AddImage("Button1", "Image/button.bmp", 122, 62, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Loading Scene Image", "Image/loading.bmp", WINSIZE_TITLE_X, WINSIZE_TITLE_Y);

	ImageManager::GetSingleton()->AddImage("SelectTileEdge", "Image/selectTileEdge.bmp", 64, 64, 2, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("SampleTile", "Image/maptiles.bmp", 640, 288, SAMPLE_TILE_X, SAMPLE_TILE_Y);
	ImageManager::GetSingleton()->AddImage("SaveLoadButton", "Image/button2.bmp", 300, 140, 2, 2);
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

		glOrtho(-2, 2, -1, 1, 1, -1);

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
