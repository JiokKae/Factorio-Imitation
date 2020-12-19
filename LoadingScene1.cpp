#include "LoadingScene1.h"
#include "Image.h"

HRESULT LoadingScene1::Init()
{
	this->SetUseBackBuffer(true);

	img = ImageManager::GetSingleton()->FindImage("Loading Scene Image");

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
		img->Render(hdc, 0, 0, width, height);
}
