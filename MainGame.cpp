#include "MainGame.h"
#include "Image.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "TileMapToolScene.h"
#include "TenCubeSpaceScene.h"
#include "LightingScene.h"
#include "LoadingScene1.h"
#include "TextRenderer.h"

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
	TextureManager::GetSingleton()->Init();

	// Add Sound
	SoundManager::GetSingleton()->AddSound("DarkWaltz", "Sound/Dark Waltz.mp3", true, false);

	// Add Scene
	SceneManager::GetSingleton()->AddScene("TitleScene", new TitleScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("PlayScene", new PlayScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("TileMapToolScene", new TileMapToolScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("TenCubeSpaceScene", new TenCubeSpaceScene(900, 900));
	SceneManager::GetSingleton()->AddScene("LightingScene", new LightingScene(900, 900));
	
	SceneManager::GetSingleton()->AddLoadingScene("LoadingScene1", new LoadingScene1(1600, 900));

	ImageLoad();

	// Change StartScene
	SceneManager::GetSingleton()->ChangeScene("PlayScene");

	TimerManager::GetSingleton()->SetTargetFPS(0);

	isInit = true;
	return S_OK;
}

void MainGame::Release()
{
	// Singleton Release
	TextureManager::GetSingleton()->Release();
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
	ImageManager::GetSingleton()->AddImage("TitleSceneImage", "core/graphics/background-image.bmp", WINSIZE_TITLE_X, WINSIZE_TITLE_Y);
	ImageManager::GetSingleton()->AddImage("Button1", "Image/button.bmp", 122, 62, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Loading Scene Image", "Image/loading.bmp", WINSIZE_TITLE_X, WINSIZE_TITLE_Y);

	ImageManager::GetSingleton()->AddImage("SelectTileEdge", "Image/selectTileEdge.bmp", 64, 64, 2, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("SampleTile", "Image/maptiles.bmp", 640, 288, SAMPLE_TILE_X, SAMPLE_TILE_Y);
	ImageManager::GetSingleton()->AddImage("SaveLoadButton", "Image/button2.bmp", 300, 140, 2, 2);

	TextureManager::GetSingleton()->AddTexture("Character/hr-level1_idle",				"base/graphics/entity/character/hr-level1_idle.png");
	TextureManager::GetSingleton()->AddTexture("Character/hr-level1_idle_shadow",		"base/graphics/entity/character/hr-level1_idle_shadow.png");
	TextureManager::GetSingleton()->AddTexture("Character/hr-level1_running",			"base/graphics/entity/character/hr-level1_running.png");
	TextureManager::GetSingleton()->AddTexture("Character/hr-level1_running_shadow",	"base/graphics/entity/character/hr-level1_running_shadow.png");
	TextureManager::GetSingleton()->AddTexture("Character/hr-level1_mining_tool",		"base/graphics/entity/character/hr-level1_mining_tool.png");
	TextureManager::GetSingleton()->AddTexture("Character/hr-level1_mining_tool_shadow", "base/graphics/entity/character/hr-level1_mining_tool_shadow.png");
	
	TextureManager::GetSingleton()->AddTexture("UI/CharacterUI",						"Image/UI/CharacterUI.png");
	TextureManager::GetSingleton()->AddTexture("UI/InventorySlotUI",					"Image/UI/InventorySlotUI.png");

	TextureManager::GetSingleton()->AddTexture("Terrain/Dirt_1",						"base/graphics/terrain/hr-dirt-1.png");
	TextureManager::GetSingleton()->AddTexture("White",									"core/graphics/white-square.png");

	TextureManager::GetSingleton()->AddTexture("IronOre", "base/graphics/entity/iron-ore/hr-iron-ore.png");

	TextureManager::GetSingleton()->AddTexture("Checker",								"Image/Checker.png");
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_WINDOWPOSCHANGED:
		if (isInit)
			Render();
		break;
	case WM_SYSCOMMAND:
		switch (wParam & 0xfff0) {
		case SC_MOVE:
		case SC_SIZE:
			TimerManager::GetSingleton()->SetIsSC_MOVE(true);
			break;
		}
		break;
	case WM_MOUSEMOVE:
	{
		Scene* scene = (Scene*)SceneManager::GetSingleton()->currScene;
		if (scene)
		{		
			g_ptMouse.x = LOWORD(lParam);
			g_ptMouse.y = scene->GetHeight() - HIWORD(lParam);
		}
		break;
	}
	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-1, 1, -1, 1, 1, -1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		{
			Scene* scene = (Scene*)SceneManager::GetSingleton()->currScene;
			if(scene)
				scene->SetWidthHeight(LOWORD(lParam), HIWORD(lParam));
		}
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
