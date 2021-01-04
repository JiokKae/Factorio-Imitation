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
	SoundManager::GetSingleton()->AddSound("RotateMedium", "core/sound/rotate-medium.ogg", false, false);

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

	TimerManager::GetSingleton()->SetTargetFPS(120);

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
	ImageManager* imgManager = ImageManager::GetSingleton();

	imgManager->AddImage("TitleSceneImage",			"graphics/background-image.bmp", WINSIZE_TITLE_X, WINSIZE_TITLE_Y);
	imgManager->AddImage("Button1",					"graphics/button.bmp", 122, 62, 1, 2, true, RGB(255, 0, 255));
	imgManager->AddImage("Loading Scene graphics",	"graphics/loading.bmp", WINSIZE_TITLE_X, WINSIZE_TITLE_Y);

	imgManager->AddImage("SelectTileEdge",	"graphics/selectTileEdge.bmp", 64, 64, 2, 2, true, RGB(255, 0, 255));
	imgManager->AddImage("SampleTile",		"graphics/maptiles.bmp", 640, 288, SAMPLE_TILE_X, SAMPLE_TILE_Y);
	imgManager->AddImage("SaveLoadButton",	"graphics/button2.bmp", 300, 140, 2, 2);

	TextureManager* textureManager = TextureManager::GetSingleton();

	textureManager->AddTexture("Character/hr-level1_idle",					"graphics/entity/character/hr-level1_idle.png");
	textureManager->AddTexture("Character/hr-level1_idle_shadow",			"graphics/entity/character/hr-level1_idle_shadow.png");
	textureManager->AddTexture("Character/hr-level1_running",				"graphics/entity/character/hr-level1_running.png");
	textureManager->AddTexture("Character/hr-level1_running_shadow",		"graphics/entity/character/hr-level1_running_shadow.png");
	textureManager->AddTexture("Character/hr-level1_mining_tool",			"graphics/entity/character/hr-level1_mining_tool.png");
	textureManager->AddTexture("Character/hr-level1_mining_tool_shadow",	"graphics/entity/character/hr-level1_mining_tool_shadow.png");

	textureManager->AddTexture("Burner-mining-drill/hr-burner-mining-drill-N",			"graphics/entity/burner_mining_drill/hr-burner-mining-drill-N.png");
	textureManager->AddTexture("Burner-mining-drill/hr-burner-mining-drill-E",			"graphics/entity/burner_mining_drill/hr-burner-mining-drill-E.png");
	textureManager->AddTexture("Burner-mining-drill/hr-burner-mining-drill-S",			"graphics/entity/burner_mining_drill/hr-burner-mining-drill-S.png");
	textureManager->AddTexture("Burner-mining-drill/hr-burner-mining-drill-W",			"graphics/entity/burner_mining_drill/hr-burner-mining-drill-W.png");
	textureManager->AddTexture("Burner-mining-drill/hr-burner-mining-drill-N-shadow",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-N-shadow.png");
	textureManager->AddTexture("Burner-mining-drill/hr-burner-mining-drill-E-shadow",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-E-shadow.png");
	textureManager->AddTexture("Burner-mining-drill/hr-burner-mining-drill-S-shadow",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-S-shadow.png");
	textureManager->AddTexture("Burner-mining-drill/hr-burner-mining-drill-W-shadow",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-W-shadow.png");

	textureManager->AddTexture("UI/CharacterUI",			"graphics/ui/CharacterUI.png");
	textureManager->AddTexture("UI/InventorySlotUI",		"graphics/ui/InventorySlotUI.png");
	textureManager->AddTexture("UI/BunerMiningDrillUI",		"graphics/ui/BunerMiningDrillUI.png");
	textureManager->AddTexture("UI/DeactiveButtonUI",		"graphics/ui/DeactiveButtonUI.png");

	textureManager->AddTexture("Icons/Hand",	"graphics/icons/hand.png");
	textureManager->AddTexture("Icons/Coal",	"graphics/icons/coal.png");

	textureManager->AddTexture("Terrain/Dirt_1",			"graphics/terrain/hr-dirt-1.png");

	textureManager->AddTexture("IronOre",					"graphics/entity/iron_ore/hr-iron-ore.png");

	textureManager->AddTexture("Checker",					"graphics/Checker.png");
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
