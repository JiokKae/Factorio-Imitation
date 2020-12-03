#include "LoadingScene1.h"
#include "Image.h"

HRESULT LoadingScene1::Init()
{
	img = ImageManager::GetSingleton()->AddImage("Loading Scene Image", "Image/loading.bmp", WINSIZE_X, WINSIZE_Y);

	return S_OK;
}

void LoadingScene1::Release()
{
}

void LoadingScene1::Update()
{
}

void LoadingScene1::Render(HDC hdc)
{
	if (img)
		img->Render(hdc, 0, 0, WINSIZE_X, WINSIZE_Y);
}
