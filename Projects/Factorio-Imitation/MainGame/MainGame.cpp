#include "MainGame.h"
#include "2DFramework/Image/Image.h"
#include "Scene/Scenes.h"
#include "GLFramework/TextRenderer/TextRenderer.h"
#include "tinyxml2/tinyxml2.h"

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

	// Add Scene
	SceneManager::GetSingleton()->AddScene("TitleScene", new TitleScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("PlayScene", new PlayScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("TileMapToolScene", new TileMapToolScene(1600, 900));
	SceneManager::GetSingleton()->AddScene("TenCubeSpaceScene", new TenCubeSpaceScene(900, 900));
	SceneManager::GetSingleton()->AddScene("LightingScene", new LightingScene(900, 900));
	
	SceneManager::GetSingleton()->AddLoadingScene("LoadingScene1", new LoadingScene1(1600, 900));

	LoadResource();

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

void MainGame::LoadResource()
{
	std::map<std::string, std::function<void(tinyxml2::XMLElement*, const std::string&)>> item_callback;
	item_callback.emplace("texture", [](tinyxml2::XMLElement* item, const std::string& group_name) {
		auto name = item->Attribute("name");
		auto path = item->Attribute("path");
		bool mipmap = item->FindAttribute("mipmap") ? item->FindAttribute("mipmap")->BoolValue() : true;
		bool flip = item->FindAttribute("flip") ? item->FindAttribute("flip")->BoolValue() : true;
		auto filter = item->Attribute("filter") ? item->Attribute("filter") : "";
		TextureManager::GetSingleton()->AddTexture(std::format("{}{}", group_name, name), path, mipmap, flip, filter);
	});
	item_callback.emplace("sound", [](tinyxml2::XMLElement* item, const std::string& group_name) {
		auto name = item->Attribute("name");
		auto path = item->Attribute("path");
		bool is_bgm = item->Attribute("is_bgm", "true");
		bool is_loop = item->Attribute("is_loop", "true");
		SoundManager::GetSingleton()->AddSound(std::format("{}{}", group_name, name), path, is_bgm, is_loop);
	});

	tinyxml2::XMLDocument doc;
	doc.LoadFile("resource.xml");

	auto resource = doc.FirstChildElement("resource");

	for (auto groups = resource->FirstChildElement("groups"); groups; groups = groups->NextSiblingElement("groups"))
	{
		auto form{ groups->Attribute("item") };
		if (form == nullptr)
			continue;

		auto callback_itr{ item_callback.find(form) };
		if (callback_itr == item_callback.end())
			continue;

		for (auto group = groups->FirstChildElement("group"); group; group = group->NextSiblingElement("group"))
		{
			const std::string group_name{ group->Attribute("name") ? std::format("{}/", group->Attribute("name")) : "" };
			for (auto item = group->FirstChildElement(form); item; item = item->NextSiblingElement(form))
				callback_itr->second(item, group_name);
		}
	}

	ImageManager* imgManager = ImageManager::GetSingleton();

	imgManager->AddImage("TitleSceneImage",		"graphics/background-image.bmp",	WINSIZE_TITLE_X, WINSIZE_TITLE_Y);
	imgManager->AddImage("Loading Scene graphics",	"graphics/loading.bmp",			WINSIZE_TITLE_X, WINSIZE_TITLE_Y);

	imgManager->AddImage("SelectTileEdge",	"graphics/selectTileEdge.bmp",	64, 64, 2, 2, true, RGB(255, 0, 255));
	imgManager->AddImage("SampleTile",	"graphics/maptiles.bmp",	640, 288, SAMPLE_TILE_X, SAMPLE_TILE_Y);
	imgManager->AddImage("SaveLoadButton",	"graphics/button2.bmp",		300, 140, 2, 2);

	imgManager->AddImage("MainMenu",	"graphics/ui/MainMenu.bmp",		352, 154, 1, 1);
	imgManager->AddImage("NewGameButton",	"graphics/ui/NewGameButton.bmp",	329, 174, 1, 3);
	imgManager->AddImage("ExitButton",	"graphics/ui/ExitButton.bmp",		122, 129, 1, 3);
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
