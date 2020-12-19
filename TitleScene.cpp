#include "TitleScene.h"
#include "Image.h"
#include "Button.h"
#include "ButtonFunction.h"

HRESULT TitleScene::Init()
{
	this->SetUseBackBuffer(true);
	SetWindowSize(50, 50, WINSIZE_TITLE_X, WINSIZE_TITLE_Y);

	img = ImageManager::GetSingleton()->AddImage("TitleSceneImage", "Image/bin.bmp", WINSIZE_X, WINSIZE_Y);
	SoundManager::GetSingleton()->Play("DarkWaltz", 0.6f);

	button1 = new Button();
	ImageManager::GetSingleton()->AddImage("Button1", "Image/button.bmp", 122, 62, 1, 2, true, RGB(255,0,255));
	button1->Init("Button1", WINSIZE_X / 2, WINSIZE_Y - 300, { 0, 1 }, { 0, 0 });
	
	quitButton = new Button();
	quitButton->Init("Button1", WINSIZE_X / 2, WINSIZE_Y - 100, { 0, 1 }, { 0, 0 });

	Argument* arg = new Argument;
	arg->a = string("TileMapToolScene");
	arg->b = string("LoadingScene1");

	button1->SetButtonFunc(ButtonFunction::ChangeScene, (void*)arg);
	quitButton->SetButtonFunc(ButtonFunction::QuitProgram, nullptr);

	Sleep(300);

	return S_OK;
}

void TitleScene::Release()
{
	SoundManager::GetSingleton()->Stop("DarkWaltz");

	SAFE_RELEASE(button1);
	SAFE_RELEASE(quitButton);
}

void TitleScene::Update()
{
	if (button1)
		button1->Update();
	if (quitButton)
		quitButton->Update();
}

void TitleScene::Render(HDC hdc)
{
	if (img)
		img->Render(hdc, 0, 0, WINSIZE_X, WINSIZE_Y);
	if (button1)
		button1->Render(hdc);
	if (quitButton)
		quitButton->Render(hdc);
}
