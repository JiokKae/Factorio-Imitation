#include "TitleScene.h"
#include "Image.h"
#include "Button.h"
#include "ButtonFunction.h"

HRESULT TitleScene::Init()
{
	this->SetUseBackBuffer(true);
	SetWindowSize(50, 50, width, height);

	img = ImageManager::GetSingleton()->FindImage("TitleSceneImage");

	SoundManager::GetSingleton()->Play("DarkWaltz", 0.6f);

	button1 = new Button();
	button1->Init("Button1", width / 2, height - 300, { 0, 1 }, { 0, 0 });
	button1->SetButtonFunc(ButtonFunction::ChangeScene, Argument_Kind::ChangeSceneArgument, new ChangeSceneArgument("TileMapToolScene", "LoadingScene1"));

	TenCubeButton = new Button();
	TenCubeButton->Init("Button1", width / 2, height - 200, { 0, 1 }, { 0, 0 });
	TenCubeButton->SetButtonFunc(ButtonFunction::ChangeScene, Argument_Kind::ChangeSceneArgument, new ChangeSceneArgument("LightingScene", "LoadingScene1"));

	quitButton = new Button();
	quitButton->Init("Button1", width / 2, height - 100, { 0, 1 }, { 0, 0 });
	quitButton->SetButtonFunc(ButtonFunction::QuitProgram);

	Sleep(350);

	return S_OK;
}

void TitleScene::Release()
{
	SoundManager::GetSingleton()->Stop("DarkWaltz");

	SAFE_RELEASE(button1);
	SAFE_RELEASE(TenCubeButton);
	SAFE_RELEASE(quitButton);
}

void TitleScene::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_ESCAPE))
		SendMessage(g_hWnd, WM_DESTROY, 0, 0);

	if (button1)
		button1->Update();
	if (TenCubeButton)
		TenCubeButton->Update();
	if (quitButton)
		quitButton->Update();
}

void TitleScene::Render(HDC hdc)
{
	if (img)
		img->Render(hdc, 0, 0, WINSIZE_TITLE_X, WINSIZE_TITLE_Y);
	if (button1)
		button1->Render(hdc);
	if (TenCubeButton)
		TenCubeButton->Render(hdc);
	if (quitButton)
		quitButton->Render(hdc);
}
