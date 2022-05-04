#pragma once
// // SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.
// 이전 Windows 플랫폼용 애플리케이션을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
// WinSDKVer.h를 포함하고 _WIN32_WINNT를 지원하려는 플랫폼으로 설정합니다.
#include <SDKDDKVer.h>
//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C++ library 
#include <vector>
#include <map>
#include <string>

// Singletons
#include "Manager/KeyManager.h"
#include "Timer/TimerManager.h"
#include "Image/ImageManager.h"
#include "Manager/SoundManager.h"

extern HWND g_hWnd;
extern float g_time;
extern bool g_hWndFocus;

#define SAFE_DELETE(p) 			{ if (p) delete p, p = nullptr; }
#define SAFE_ARR_DELETE(p) 		{ if (p) delete[] p, p = nullptr; }
#define SAFE_RELEASE(p) 		{ if (p) p->Release(), delete p, p = nullptr; }
#define SAFE_ARR_RELEASE(p, size)	{ if (p) { for (int i = 0; i < size; i++) { p[i].Release(); } delete[] p, p = nullptr; } }

enum class Argument_Kind {
	None,
	ChangeSceneArgument,
	TileInfoArgument,
};

struct ChangeSceneArgument
{
	string sceneName;
	string loadingSceneName;

	ChangeSceneArgument(string sceneName, string loadingSceneName);
};

typedef struct tagTile TILE_INFO;
struct TileInfoArgument
{
	TILE_INFO* tileInfo;

	TileInfoArgument(TILE_INFO* lpTileInfo);

};

typedef struct tagFRECT
{
	float left;
	float top;
	float right;
	float bottom;

	tagFRECT(float left = 0, float top = 0, float right = 0, float bottom = 0);
	const tagFRECT operator+(const tagFRECT& rect) const;
	template<typename T>
	const tagFRECT operator*(const T& scalar);

} FRECT, * LPFRECT;

inline RECT GetWindowRect()
{
	RECT rc;
	POINT lt, rb;
	GetClientRect(g_hWnd, &rc);
	// 클라이언트 크기를 받아옴
	lt.x = rc.left;
	lt.y = rc.top;
	rb.x = rc.right;
	rb.y = rc.bottom;
	// 받아온 클라이언트 크기를좌표로 입력
	ClientToScreen(g_hWnd, &lt);
	ClientToScreen(g_hWnd, &rb);
	// 클라이언트 내 좌표를 윈도우상 좌표로 변환
	rc.left = lt.x;
	rc.top = lt.y;
	rc.right = rb.x;
	rc.bottom = rb.y;

	return rc;
}