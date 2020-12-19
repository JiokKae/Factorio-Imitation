// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once
// // SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.
// 이전 Windows 플랫폼용 애플리케이션을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
// WinSDKVer.h를 포함하고 _WIN32_WINNT를 지원하려는 플랫폼으로 설정합니다.
#include <SDKDDKVer.h>
//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <GL/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#include "KeyManager.h"
#include "TimerManager.h"
#include "ImageManager.h"
#include "SceneManager.h"
#include "SoundManager.h"

#define WINSIZE_X	900
#define WINSIZE_Y	900
#define WINSIZE_TITLE_X		1600
#define WINSIZE_TITLE_Y		900
#define WINSIZE_TILE_MAP_X	1600
#define WINSIZE_TILE_MAP_Y	900
#define SAFE_DELETE(p) 		{if (p) delete p, p = nullptr; }
#define SAFE_ARR_DELETE(p) 	{if (p) delete[] p, p = nullptr; }
#define SAFE_RELEASE(p) 	{if (p) p->Release(), delete p, p = nullptr; }

struct Argument {
	string a;
	string b;
};

extern HWND			g_hWnd;
extern HINSTANCE	g_hInstance;
extern POINT		g_ptMouse;
extern float		g_time;
extern float		g_mousezDelta;

inline void SetWindowSize(int startX, int startY, int sizeX, int sizeY)
{
	// 윈도우 작업영역 지정
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = sizeX;
	rc.bottom = sizeY;

	// 실제 윈도우 크기 받아오기
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	// 이동
	MoveWindow(g_hWnd, startX, startY, rc.right - rc.left, rc.bottom - rc.top, true);
}

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

template <typename T>
inline T Clamp(T value, T min, T max)
{
	if (value > max) return max;
	else if (value < min) return min;
	return value;
}

template <typename T>
inline T Lerp(T a, T b, float v)
{
	return a + (b - a) * v;
}