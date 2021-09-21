#pragma once
#include "framework.h"

class Animation;
class Image
{
public:
	enum IMAGE_LOAD_KIND
	{
		RESOURCE,
		FILE,
		EMPTY,
		END
	};

	typedef struct tagImageInfo
	{
		DWORD	resID;		// 리소스의 고유한 아이디
		HDC	hMemDC;		// 그리기를 주관하는 핸들 (메모리 관리)
		HBITMAP	hBitmap;	// 이미지 정보
		HBITMAP hOldBit;	// 기존 이미지 정보
		int	width;		// 이미지 가로 크기
		int	height;		// 이미지 세로 크기
		BYTE	loadType;	// 로드 타입

		// Blend
		HDC hBlendDC;
		HBITMAP hBlendBitmap;
		HBITMAP hOldBlendBit;

		// Animation에 필요한 정보
		int maxFrameX;		// 최대 프레임 인덱스
		int maxFrameY;		
		int currFrameX;		// 현재 프레임 인덱스
		int currFrameY;
		int frameWidth;		// 프레임별 크기
		int frameHeight;

		// Animation 적용시 위치
		float x;
		float y;

		tagImageInfo() {
			resID = 0;
			hMemDC = NULL;
			hBitmap = NULL;
			hOldBit = NULL;
			width = 0;
			height = 0;
			loadType = IMAGE_LOAD_KIND::EMPTY;

			maxFrameX = 0;
			maxFrameY = 0;
			currFrameX = 0;
			currFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			x = y = 0;
			hBlendDC = NULL;
			hBlendBitmap = NULL;
			hOldBlendBit = NULL;
		};

	} IMAGE_INFO, *LPIMAGE_INFO;

private:
	IMAGE_INFO*	imageInfo;		// 이미지 정보 구조체 포인터
	//LPIMAGE_INFO imageInfo;

	char* fileName;				// 파일 경로 + 이름
	bool isTrans;				// 투명화 여부
	COLORREF transColor;		// 투명화할 색

	BLENDFUNCTION blendFunc;

public:	
	//빈 비트맵 이미지를 만든다.
	HRESULT Init(int width, int height);

	// 리소스로부터 데이터를 읽어서 비트맵을 만든다.
	HRESULT Init(const DWORD resID, int width, int height,
		bool isTrans = FALSE, COLORREF transColor = FALSE);

	// 파일로부터 데이터를 읽어서 비트맵을 만든다.
	HRESULT Init(const char* fileName, int width, int height,
		bool isTrans = FALSE, COLORREF transColor = FALSE);

	// 파일로부터 데이터를 읽어서 비트맵을 만든다. (애니메이션)
	HRESULT Init(const char* fileName, int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTrans = FALSE, COLORREF transColor = FALSE);

	// 메모리 해제
	void Release();

	void Render(HDC hdc, int destX, int destY, int sizeX, int sizeY);
	void Render(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight);
	void FrameRender(HDC hdc, int destX, int destY, int currFrameX, int currFrameY);
	void AlphaRender(HDC hdc, int destX, int destY, BYTE alpha);
	void AnimationRender(HDC hdc, int destX, int destY, Animation* ani);

	HDC GetMemDC()			{ return imageInfo->hMemDC; }
	int GetMaxFrameX()		{ return imageInfo->maxFrameX; }
	int GetWidth()			{ return imageInfo->width; }
	int GetHeight()			{ return imageInfo->height; }
	int GetFrameWidth()		{ return imageInfo->frameWidth; }
	int GetFrameHeight()	{ return imageInfo->frameHeight; }

	Image();
	~Image();
};

