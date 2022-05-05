#include "TitleScene.h"
#include "2DFramework/Image/Image.h"
#include "2DFramework/Button/Button.h"
#include "MainGame/Button/ButtonFunction.h"
#include "framework.h"

HRESULT TitleScene::Init()
{
	this->SetUseBackBuffer(true);
	SetWindowSize((1920 - width) / 2, (1080 - height) / 2, width, height);

	img = ImageManager::GetSingleton()->FindImage("TitleSceneImage");
	mainMenuImage = ImageManager::GetSingleton()->FindImage("MainMenu");
	SoundManager::GetSingleton()->Play("Ambient/Expansion", 0.6f);

	gameStartButton = new Button();
	gameStartButton->Init("NewGameButton", width / 2, height - 500 + 43, { 0, 2 }, { 0, 1 });
	gameStartButton->SetButtonFunc(ButtonFunction::ChangeScene, Argument_Kind::ChangeSceneArgument, new ChangeSceneArgument("PlayScene", ""));

	button1 = new Button();
	button1->Init("NewGameButton", width / 2, height - 400, { 0, 2 }, { 0, 1 });
	button1->SetButtonFunc(ButtonFunction::ChangeScene, Argument_Kind::ChangeSceneArgument, new ChangeSceneArgument("TileMapToolScene", "LoadingScene1"));

	TenCubeButton = new Button();
	TenCubeButton->Init("NewGameButton", width / 2, height - 300, { 0, 2 }, { 0, 1 });
	TenCubeButton->SetButtonFunc(ButtonFunction::ChangeScene, Argument_Kind::ChangeSceneArgument, new ChangeSceneArgument("TenCubeSpaceScene",""));

	lightingButton = new Button();
	lightingButton->Init("NewGameButton", width / 2, height - 200, { 0, 2 }, { 0, 1 });
	lightingButton->SetButtonFunc(ButtonFunction::ChangeScene, Argument_Kind::ChangeSceneArgument, new ChangeSceneArgument("LightingScene", ""));

	quitButton = new Button();
	quitButton->Init("ExitButton", width / 2 - 108, height - 100 - 304, { 0, 2 }, { 0, 1 });
	quitButton->SetButtonFunc(ButtonFunction::QuitProgram);

	Sleep(350);

	return S_OK;
}

void TitleScene::Release()
{
	SoundManager::GetSingleton()->Stop("Ambient/Expansion");

	SAFE_RELEASE(gameStartButton);
	SAFE_RELEASE(lightingButton);
	SAFE_RELEASE(button1);
	SAFE_RELEASE(TenCubeButton);
	SAFE_RELEASE(quitButton);
}

void TitleScene::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_ESCAPE))
		SendMessage(g_hWnd, WM_DESTROY, 0, 0);

	if (gameStartButton)
		gameStartButton->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);

	if (g_debuggingMode)
	{
		if (button1)
			button1->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);
		if (TenCubeButton)
			TenCubeButton->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);
		if (lightingButton)
			lightingButton->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);
	}

	if (quitButton)
		quitButton->Update(g_ptMouse.x, g_ptMouse.y, WINSIZE_Y);
}

void TitleScene::Render(HDC hdc)
{
	if (img)
		img->Render(hdc, 0, 0, WINSIZE_TITLE_X, WINSIZE_TITLE_Y);

	if (mainMenuImage)
		mainMenuImage->FrameRender(hdc, WINSIZE_TITLE_X / 2, WINSIZE_TITLE_Y / 2, 0, 0);

	if (gameStartButton)
		gameStartButton->Render(hdc);

	if (g_debuggingMode)
	{
		if (button1)
			button1->Render(hdc);
		if (TenCubeButton)
			TenCubeButton->Render(hdc);
		if (lightingButton)
			lightingButton->Render(hdc);
	}

	if (quitButton)
		quitButton->Render(hdc);
}
