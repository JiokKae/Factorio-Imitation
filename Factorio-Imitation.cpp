﻿// Factorio-Imitation.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MainGame.h"

// 전역 변수:
HINSTANCE	g_hInstance;	// 현재 인스턴스입니다.
HWND		g_hWnd;
MainGame	g_mainGame;
LPSTR		g_lpszClass = (LPSTR)TEXT("Factorio-Imitation");
POINT		g_ptMouse;
float		g_mousezDelta;
glm::vec2	g_cursorPosition;	// 마우스 커서의 포지션
glm::vec2	g_cursorCoord;		// 마우스 커서의 좌표
glm::vec2	g_currScreenSize;	// 현재 씬 윈도우의 크기
float		g_time;
bool		g_debuggingMode;
ItemSpec	g_itemSpecs[] = {
//		name						buildable	coordSize	direction	passable	fuel,	fuelValue
//															Count
	{	"Assembling Machine 1",		true,		{ 3, 3 },	1,			FALSE,		FALSE,	NULL,		},
	{	"Burner Mining Drill",		true,		{ 2, 2 },	4,			FALSE,		FALSE,	NULL,		},
	{	"Iron Plate",				FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		},
	{	"Coal",						FALSE,		{ 0, 0 },	NULL,		FALSE,		true,	4000,		},
	{	"Transport Belt",			true,		{ 1, 1 },	4,			true,		FALSE,	NULL,		},
	{	"Burner Inserter",			true,		{ 1, 1 },	4,			FALSE,		FALSE,	NULL,		},
	{	"Wood",						FALSE,		{ 0, 0 },	NULL,		FALSE,		true,	2000,		},
	{	"Stone Furnace",			true,		{ 2, 2 },	1,			FALSE,		FALSE,	NULL,		},
	{	"Solid Fuel",				FALSE,		{ 0, 0 },	NULL,		FALSE,		true,	12000,		},
	{	"Rocket Fuel",				FALSE,		{ 0, 0 },	NULL,		FALSE,		true,	100000,		},
	{	"Nuclear Fuel",				FALSE,		{ 0, 0 },	NULL,		FALSE,		true,	1210000,	},
	{	"Iron Ore",					FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		},
	{	"Copper Ore",				FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		},
	{	"Copper Plate",				FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		},
	{	"Stone",					FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		},
	{	"Stone Brick",				FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		},
	{	"Steel Plate",				FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		},
	{	"Wooden Chest",				true,		{ 1, 1 },	1,			FALSE,		FALSE,	NULL,		},
	{	"Fast Transport Belt",		true,		{ 1, 1 },	4,			true,		FALSE,	NULL,		},
	{	"Express Transport Belt",	true,		{ 1, 1 },	4,			true,		FALSE,	NULL,		},
};
enum ItemEnum;	// 정의 피킹용

const char* g_directionToLpChar[] = {
	"-N",
	"-E",
	"-S",
	"-W",
};


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR	lpCmdLine,
					  _In_ int		nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	g_mainGame.Init();

	// 메시지 큐에 있는 메시지 처리
	MSG message;
	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
				break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			if (TimerManager::GetSingleton()->Update())
			{
				g_mainGame.Update();
				TimerManager::GetSingleton()->CheckUpdateTime();
				g_mainGame.Render();
				TimerManager::GetSingleton()->CheckRenderTime();
				g_time += TimerManager::GetSingleton()->GetTimeElapsed();
			}
		}
	}

	g_mainGame.Release();

	return (int)message.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_lpszClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInstance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return g_mainGame.MainProc(hWnd, message, wParam, lParam);
}
