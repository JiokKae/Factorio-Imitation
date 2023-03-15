#include "MainGame.h"
#include "2DFramework/Image/Image.h"
#include "Scene/Scenes.h"
#include "GLFramework/TextRenderer/TextRenderer.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);

	#pragma region Win OpenGL Init
	PIXELFORMATDESCRIPTOR pfd{
		.nSize = sizeof(pfd),
		.nVersion = 1,
		.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER_DONTCARE,
		.iPixelType = PFD_TYPE_RGBA,
		.cColorBits = 32,
	};
	SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);
	glewInit();
	#pragma endregion
	
	// Singleton Init
	TimerManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init(g_hWnd);
	SoundManager::GetSingleton()->Init();
	TextureManager::GetSingleton()->Init();
	RecipeManager::GetSingleton()->Init();
	DebugHelper::GetSingleton()->Init();

	// TextRenderer Init
	TextRenderer::GetSingleton()->Init();
	TextRenderer::GetSingleton()->Load("Fonts/NotoSans-Bold.ttf", 24);
	SoundLoad();

	// Add Scene
	SceneManager::GetSingleton()->AddScene("TitleScene", new TitleScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("PlayScene", new PlayScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("TileMapToolScene", new TileMapToolScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("TenCubeSpaceScene", new TenCubeSpaceScene(900, 900));
	SceneManager::GetSingleton()->AddScene("LightingScene", new LightingScene(900, 900));
	
	SceneManager::GetSingleton()->AddLoadingScene("LoadingScene1", new LoadingScene1(1600, 900));

	ImageLoad();

	// Change StartScene
	SceneManager::GetSingleton()->ChangeScene("TitleScene");

	TimerManager::GetSingleton()->SetTargetFPS(120);
	isInit = true;

	return S_OK;
}

void MainGame::Release()
{
	// Singleton Release
	DebugHelper::GetSingleton()->Release();
	TextRenderer::GetSingleton()->Release();
	RecipeManager::GetSingleton()->Release();
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
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F5))
		g_debuggingMode = !g_debuggingMode;
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

	imgManager->AddImage("TitleSceneImage",		"graphics/background-image.bmp",	WINSIZE_TITLE_X, WINSIZE_TITLE_Y);
	imgManager->AddImage("Loading Scene graphics",	"graphics/loading.bmp",			WINSIZE_TITLE_X, WINSIZE_TITLE_Y);

	imgManager->AddImage("SelectTileEdge",	"graphics/selectTileEdge.bmp",	64, 64, 2, 2, true, RGB(255, 0, 255));
	imgManager->AddImage("SampleTile",	"graphics/maptiles.bmp",	640, 288, SAMPLE_TILE_X, SAMPLE_TILE_Y);
	imgManager->AddImage("SaveLoadButton",	"graphics/button2.bmp",		300, 140, 2, 2);

	imgManager->AddImage("MainMenu",	"graphics/ui/MainMenu.bmp",		352, 154, 1, 1);
	imgManager->AddImage("NewGameButton",	"graphics/ui/NewGameButton.bmp",	329, 174, 1, 3);
	imgManager->AddImage("ExitButton",	"graphics/ui/ExitButton.bmp",		122, 129, 1, 3);

	TextureManager* textureManager( TextureManager::GetSingleton() );

	textureManager->AddTexture("Entity/Character-level1_idle",			"graphics/entity/character/hr-level1_idle.png");
	textureManager->AddTexture("Entity/Character-level1_idle_shadow",		"graphics/entity/character/hr-level1_idle_shadow.png");
	textureManager->AddTexture("Entity/Character-level1_running",			"graphics/entity/character/hr-level1_running.png");
	textureManager->AddTexture("Entity/Character-level1_running_shadow",		"graphics/entity/character/hr-level1_running_shadow.png");
	textureManager->AddTexture("Entity/Character-level1_mining_tool",		"graphics/entity/character/hr-level1_mining_tool.png");
	textureManager->AddTexture("Entity/Character-level1_mining_tool_shadow",	"graphics/entity/character/hr-level1_mining_tool_shadow.png");

	textureManager->AddTexture("Entity/BurnerMiningDrill-N",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-N.png");
	textureManager->AddTexture("Entity/BurnerMiningDrill-E",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-E.png");
	textureManager->AddTexture("Entity/BurnerMiningDrill-S",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-S.png");
	textureManager->AddTexture("Entity/BurnerMiningDrill-W",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-W.png");
	textureManager->AddTexture("Entity/BurnerMiningDrill-N-shadow",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-N-shadow.png");
	textureManager->AddTexture("Entity/BurnerMiningDrill-E-shadow",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-E-shadow.png");
	textureManager->AddTexture("Entity/BurnerMiningDrill-S-shadow",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-S-shadow.png");
	textureManager->AddTexture("Entity/BurnerMiningDrill-W-shadow",	"graphics/entity/burner_mining_drill/hr-burner-mining-drill-W-shadow.png");

	textureManager->AddTexture("Entity/AssemblingMachine1",		"graphics/entity/assembling_machine_1/hr-assembling-machine-1.png");
	textureManager->AddTexture("Entity/AssemblingMachine1-shadow",	"graphics/entity/assembling_machine_1/hr-assembling-machine-1-shadow.png");

	textureManager->AddTexture("Entity/TransportBelt",		"graphics/entity/transport_belt/hr-transport-belt.png",			true, true, GL_NEAREST);
	textureManager->AddTexture("Entity/FastTransportBelt",		"graphics/entity/fast_transport_belt/hr-fast-transport-belt.png",	true, true, GL_NEAREST);
	textureManager->AddTexture("Entity/ExpressTransportBelt",	"graphics/entity/express_transport_belt/hr-express-transport-belt.png", true, true, GL_NEAREST);

	textureManager->AddTexture("Entity/BurnerInserter-platform",		"graphics/entity/burner_inserter/hr-burner-inserter-platform.png");
	textureManager->AddTexture("Entity/BurnerInserter-handBase",		"graphics/entity/burner_inserter/hr-burner-inserter-hand-base.png");
	textureManager->AddTexture("Entity/BurnerInserter-handBase-shadow",	"graphics/entity/burner_inserter/hr-burner-inserter-hand-base-shadow.png");
	textureManager->AddTexture("Entity/BurnerInserter-handClosed",		"graphics/entity/burner_inserter/hr-burner-inserter-hand-closed.png");
	textureManager->AddTexture("Entity/BurnerInserter-handClosed-shadow",	"graphics/entity/burner_inserter/hr-burner-inserter-hand-closed-shadow.png");
	textureManager->AddTexture("Entity/BurnerInserter-handOpen",		"graphics/entity/burner_inserter/hr-burner-inserter-hand-open.png");
	textureManager->AddTexture("Entity/BurnerInserter-handOpen-shadow",	"graphics/entity/burner_inserter/hr-burner-inserter-hand-open-shadow.png");

	textureManager->AddTexture("Entity/StoneFurnace",		"graphics/entity/stone_furnace/hr-stone-furnace.png");
	textureManager->AddTexture("Entity/StoneFurnace-shadow",	"graphics/entity/stone_furnace/hr-stone-furnace-shadow.png");
	textureManager->AddTexture("Entity/StoneFurnace-fire",		"graphics/entity/stone_furnace/hr-stone-furnace-fire.png");

	textureManager->AddTexture("Entity/IronOre",			"graphics/entity/iron_ore/hr-iron-ore.png");
	textureManager->AddTexture("Entity/Coal",			"graphics/entity/coal/hr-coal.png");
	textureManager->AddTexture("Entity/CopperOre",			"graphics/entity/copper_ore/hr-copper-ore.png");
	textureManager->AddTexture("Entity/Stone",			"graphics/entity/stone/hr-stone.png");

	textureManager->AddTexture("Entity/WoodenChest",	"graphics/entity/wooden_chest/hr-wooden-chest.png");
	textureManager->AddTexture("Entity/WoodenChest-shadow",	"graphics/entity/wooden_chest/hr-wooden-chest-shadow.png");
	textureManager->AddTexture("Entity/IronChest",		"graphics/entity/iron_chest/hr-iron-chest.png");
	textureManager->AddTexture("Entity/IronChest-shadow",	"graphics/entity/iron_chest/hr-iron-chest-shadow.png");
	textureManager->AddTexture("Entity/SteelChest",		"graphics/entity/steel_chest/hr-steel-chest.png");
	textureManager->AddTexture("Entity/SteelChest-shadow",	"graphics/entity/steel_chest/hr-steel-chest-shadow.png");

	textureManager->AddTexture("UI/CharacterUI",			"graphics/ui/CharacterUI.png",			false);
	textureManager->AddTexture("UI/SlotUI",				"graphics/ui/slotUI.png",			false);
	textureManager->AddTexture("UI/BunerMiningDrillUI",		"graphics/ui/BunerMiningDrillUI.png",		false);
	textureManager->AddTexture("UI/DeactiveButtonUI",		"graphics/ui/DeactiveButtonUI.png",		false);
	textureManager->AddTexture("UI/RedProgressiveBarUI",		"graphics/ui/redProgressiveBarUI.png",		false);
	textureManager->AddTexture("UI/GreenProgressiveBarUI",		"graphics/ui/greenProgressiveBarUI.png",	false);
	textureManager->AddTexture("UI/FuelSlotUI",			"graphics/ui/fuelSlotUI.png",			false);
	textureManager->AddTexture("UI/StoneFurnaceUI",			"graphics/ui/stoneFurnaceUI.png",		false);
	textureManager->AddTexture("UI/BackgroundUI",			"graphics/ui/BacgroundUI.png",			false);
	textureManager->AddTexture("UI/BackgroundNameUI",		"graphics/ui/BacgroundNameUI.png",		false);
	textureManager->AddTexture("UI/DismantleBarUI",			"graphics/ui/dismantleBarUI.png",		false);
	textureManager->AddTexture("UI/DismantleEmptyBarUI",		"graphics/ui/dismantleEmptyBarUI.png",		false);
	textureManager->AddTexture("UI/SelectRecipeAssemblingUI",	"graphics/ui/selectRecipeAssemblingUI.png",	false);
	textureManager->AddTexture("UI/RecipeButtonUI",			"graphics/ui/recipeButtonUI.png",		false);

	textureManager->AddTexture("Icons/Hand",			"graphics/icons/hand.png");
	textureManager->AddTexture("Icons/AllItems",			"graphics/icons/all-items.png");
	textureManager->AddTexture("Icons/AllItems-stroke",		"graphics/icons/all-items-stroke.png");

	textureManager->AddTexture("Terrain/Dirt_1",			"graphics/terrain/hr-dirt-1.png");

}

void MainGame::SoundLoad()
{
	SoundManager* soundManager( SoundManager::GetSingleton() );

	soundManager->AddSound("Rotate-small",	"sound/rotate-small.ogg",	false, false);
	soundManager->AddSound("Rotate-medium", "sound/rotate-medium.ogg",	false, false);
	soundManager->AddSound("Rotate-big",	"sound/rotate-big.ogg",		false, false);

	soundManager->AddSound("Build-small",	"sound/build-small.ogg",	false, false);
	soundManager->AddSound("Build-medium",	"sound/build-medium.ogg",	false, false);
	soundManager->AddSound("Build-large",	"sound/build-large.ogg",	false, false);

	soundManager->AddSound("Deconstruct-small",	"sound/deconstruct-small.ogg",	false, false);
	soundManager->AddSound("Deconstruct-medium",	"sound/deconstruct-medium.ogg",	false, false);
	soundManager->AddSound("Deconstruct-large",	"sound/deconstruct-large.ogg",	false, false);

	soundManager->AddSound("GUI-ToolButton", "sound/gui-tool-button.ogg", false, false);
	soundManager->AddSound("GUI-ButtonMini", "sound/gui-button-mini.ogg", false, false);

	soundManager->AddSound("Machine-open",	"sound/machine-open.ogg",	false, false);
	soundManager->AddSound("Machine-close", "sound/machine-close.ogg",	false, false);

	soundManager->AddSound("PickedUpItem",	"sound/picked-up-item.ogg", false, false);

	soundManager->AddSound("Ambient/Expansion", "sound/ambient/expansion.ogg", true, true);
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
		switch (wParam & 0xfff0) 
		{
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
			Scene* scene( SceneManager::GetSingleton()->currScene );
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
