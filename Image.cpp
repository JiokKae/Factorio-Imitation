#include "Image.h"
#include "Animation.h"

HRESULT Image::Init(int width, int height)
{
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);
	imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hOldBit =
		(HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = IMAGE_LOAD_KIND::EMPTY;

	imageInfo->hBlendDC = CreateCompatibleDC(hdc);
	imageInfo->hBlendBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC, imageInfo->hBlendBitmap);

	ReleaseDC(g_hWnd, hdc);

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	blendFunc.AlphaFormat = 0;	// AC_SRC_ALPHA;
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 0;

	return S_OK;
}

HRESULT Image::Init(const DWORD resID, int width, int height, bool isTrans, COLORREF transColor)
{
	return E_NOTIMPL;
}

HRESULT Image::Init(const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);
	imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName,
		IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

	imageInfo->hBlendDC = CreateCompatibleDC(hdc);
	imageInfo->hBlendBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC, imageInfo->hBlendBitmap);

	ReleaseDC(g_hWnd, hdc);

	this->isTrans = isTrans;
	this->transColor = transColor;

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	blendFunc.AlphaFormat = 0;	// AC_SRC_ALPHA;
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 0;

	return S_OK;
}

HRESULT Image::Init(const char * fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);
	imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName,
		IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	imageInfo->hOldBit =
		(HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

	imageInfo->hBlendDC = CreateCompatibleDC(hdc);
	imageInfo->hBlendBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC, imageInfo->hBlendBitmap);

	// 애니메이션 관련
	imageInfo->currFrameX = 0;
	imageInfo->currFrameY = 0;
	imageInfo->maxFrameX = maxFrameX;
	imageInfo->maxFrameY = maxFrameY;
	imageInfo->frameWidth = width / maxFrameX;
	imageInfo->frameHeight = height / maxFrameY;

	ReleaseDC(g_hWnd, hdc);

	this->isTrans = isTrans;
	this->transColor = transColor;

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

void Image::Release()
{
	if (imageInfo)
	{
		SelectObject(imageInfo->hMemDC, imageInfo->hOldBit);
		DeleteObject(imageInfo->hBitmap);
		DeleteDC(imageInfo->hMemDC);

		DeleteObject(SelectObject(imageInfo->hBlendDC, imageInfo->hOldBlendBit));
		DeleteDC(imageInfo->hBlendDC);

		delete imageInfo;
		imageInfo = nullptr;
	}

}

void Image::Render(HDC hdc, int destX, int destY, int sizeX, int sizeY)
{
	imageInfo->x = destX - (imageInfo->width / 2);
	imageInfo->y = destY - (imageInfo->height / 2);

	if (isTrans)
	{
		GdiTransparentBlt(
			hdc,
			destX, destY,
			sizeX, sizeY,

			imageInfo->hMemDC,
			0, 0,
			imageInfo->width, imageInfo->height,
			transColor);
	}
	else
	{
		BitBlt(
			hdc,						// 복사 목적지 DC
			destX, destY,				// 복사 시작 위치
			imageInfo->width,			// 원본에서 복사될 가로 크기
			imageInfo->height,			// 원본에서 복사될 세로 크기

			imageInfo->hMemDC,			// 원본 DC
			0, 0,
			SRCCOPY						// 복사 옵션
		);
	}

}

void Image::Render(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight)
{
	imageInfo->x = destX - (imageInfo->width / 2);
	imageInfo->y = destY - (imageInfo->height / 2);

	if (isTrans)
	{
		GdiTransparentBlt(
			hdc,
			destX - (srcWidth / 2),
			destY - (srcHeight / 2),	// 복사 시작 위치
			srcWidth,					// 원본에서 복사될 가로 크기
			srcHeight,					// 원본에서 복사될 세로 크기

			imageInfo->hMemDC,
			srcX, srcY,
			srcWidth, srcHeight,
			transColor);
	}
	else
	{
		BitBlt(
			hdc,						// 복사 목적지 DC
			destX - (srcWidth / 2), 
			destY - (srcHeight / 2) ,	// 복사 시작 위치
			srcWidth,					// 원본에서 복사될 가로 크기
			srcHeight,					// 원본에서 복사될 세로 크기
			imageInfo->hMemDC,			// 원본 DC
			srcX, srcY,
			SRCCOPY						// 복사 옵션
		);
	}

}

void Image::FrameRender(HDC hdc, int destX, int destY, int currFrameX, int currFrameY)
{
	imageInfo->currFrameX = currFrameX;
	imageInfo->currFrameY = currFrameY;

	imageInfo->x = destX - (imageInfo->frameWidth / 2);
	imageInfo->y = destY - (imageInfo->frameHeight / 2);

	if (isTrans)
	{
		GdiTransparentBlt(
			hdc,
			imageInfo->x, imageInfo->y,
			imageInfo->frameWidth, imageInfo->frameHeight,

			imageInfo->hMemDC,
			imageInfo->currFrameX * imageInfo->frameWidth,
			imageInfo->currFrameY * imageInfo->frameHeight,
			imageInfo->frameWidth, imageInfo->frameHeight,
			transColor);
	}
	else
	{
		BitBlt(
			hdc,						// 복사 목적지 DC
			imageInfo->x, imageInfo->y,	// 복사 시작 위치
			imageInfo->frameWidth,		// 원본에서 복사될 가로 크기
			imageInfo->frameHeight,		// 원본에서 복사될 세로 크기

			imageInfo->hMemDC,			// 원본 DC
			imageInfo->currFrameX * imageInfo->frameWidth,
			imageInfo->currFrameY * imageInfo->frameHeight,
			SRCCOPY						// 복사 옵션
		);
	}

}

void Image::AlphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	blendFunc.SourceConstantAlpha = alpha;
	if (isTrans)
	{
		BitBlt(
			imageInfo->hBlendDC, 0, 0,
			imageInfo->width, imageInfo->height,
			hdc, destX, destY,
			SRCCOPY);
		GdiTransparentBlt(
			imageInfo->hBlendDC, 0, 0,
			imageInfo->width, imageInfo->height, 
			imageInfo->hMemDC, 0, 0,
			imageInfo->width, imageInfo->height, RGB(255, 0, 255));
		AlphaBlend(
			hdc, destX, destY, 
			imageInfo->width, imageInfo->height,
			imageInfo->hBlendDC, 0, 0,
			imageInfo->width, imageInfo->height, blendFunc);
	}
	else
	{
		AlphaBlend(hdc, destX, destY, imageInfo->width, imageInfo->height,
			imageInfo->hMemDC, 0, 0, imageInfo->width, imageInfo->height, blendFunc);
	}
}

void Image::AnimationRender(HDC hdc, int destX, int destY, Animation * ani)
{
	Render(hdc, destX, destY, ani->GetFramePoint().x, ani->GetFramePoint().y, imageInfo->frameWidth, imageInfo->frameHeight);
}

Image::Image()
{
}


Image::~Image()
{
}
