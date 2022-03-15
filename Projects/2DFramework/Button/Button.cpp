#include "Button.h"
#include "../Image/Image.h"

HRESULT Button::Init(const char* imageName, int posX, int posY, POINT downFramePoint, POINT upFramePoint)
{
	pos.x = posX;
	pos.y = posY;
	ptDownFrame = downFramePoint;
	ptUpFrame = upFramePoint;

	img = ImageManager::GetSingleton()->FindImage(imageName);

	rc.left = posX - img->GetFrameWidth() / 2;
	rc.right = posX + img->GetFrameWidth() / 2;
	rc.top = posY - img->GetFrameHeight() / 2;
	rc.bottom = posY + img->GetFrameHeight() / 2;

	return S_OK;
}

HRESULT Button::Init()
{
	return E_NOTIMPL;
}

void Button::Release()
{
	if (arg)
	{
		switch (kind)
		{
		case Argument_Kind::ChangeSceneArgument:
			delete (ChangeSceneArgument*)arg;
			arg = nullptr;
			break;
		default:
			throw new std::exception( "버튼 릴리즈 처리가 되지 않았습니다" );
			break;
		}
	}
}

void Button::Update()
{
	if (PtInRect(&rc, { (int)g_ptMouse.x, WINSIZE_TITLE_Y - (int)g_ptMouse.y }))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			state = BUTTON_STATE::DOWN;
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON) && state == BUTTON_STATE::DOWN)
		{
			state = BUTTON_STATE::UP;
			SoundManager::GetSingleton()->Play("GUI-ButtonMini", 0.8f);
			// 버튼 기능 수행
			ButtonFunc();
		}
	}
	else
	{
		state = BUTTON_STATE::NONE;
	}
}

void Button::Render(HDC hdc)
{
	switch (state)
	{
	case NONE:
	case UP:
		if (img)
		{
			img->FrameRender(hdc, pos.x, pos.y, ptUpFrame.x, ptUpFrame.y);
		}
		break;

	case DOWN:
		if (img)
		{
			img->FrameRender(hdc, pos.x, pos.y, ptDownFrame.x, ptDownFrame.y);
		}
		break;
	}
}

Button::Button() 
	: state(BUTTON_STATE::NONE)
	, img(nullptr)
{
}
