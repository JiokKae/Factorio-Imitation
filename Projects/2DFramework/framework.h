#pragma once
// // SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.
// 이전 Windows 플랫폼용 애플리케이션을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
// WinSDKVer.h를 포함하고 _WIN32_WINNT를 지원하려는 플랫폼으로 설정합니다.
// #include <SDKDDKVer.h>
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

enum class Argument_Kind 
{
	None,
	ChangeSceneArgument,
	TileInfoArgument,
};

struct ChangeSceneArgument
{
	std::string sceneName;
	std::string loadingSceneName;
};

struct TILE_INFO;
struct TileInfoArgument
{
	TILE_INFO* tileInfo;
};

struct FRECT
{
	float left;
	float top;
	float right;
	float bottom;

	FRECT operator+(const FRECT& rect) const;
	FRECT operator*(float scalar) const;
} ;

inline RECT GetWindowRect()
{
	RECT clientRect;
	GetClientRect(g_hWnd, &clientRect);

	// 클라이언트 크기를 좌표로 입력
	POINT leftTop{ clientRect.left , clientRect.top };

	POINT rightBottom{ clientRect.right, clientRect.bottom };

	// 클라이언트 내 좌표를 윈도우상 좌표로 변환
	ClientToScreen(g_hWnd, &leftTop);
	ClientToScreen(g_hWnd, &rightBottom);

	clientRect.left = leftTop.x;
	clientRect.top = leftTop.y;
	clientRect.right = rightBottom.x;
	clientRect.bottom = rightBottom.y;

	return clientRect;
}

template <typename T>
inline T Clamp(const T& value, const T& min, const T& max)
{
	if (value > max) return max;
	else if (value < min) return min;
	return value;
}
