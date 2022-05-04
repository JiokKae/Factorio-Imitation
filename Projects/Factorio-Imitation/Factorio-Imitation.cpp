// Factorio-Imitation.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MainGame/MainGame.h"

// 전역 변수:
HINSTANCE	g_hInstance;	// 현재 인스턴스입니다.
HWND		g_hWnd;
bool		g_hWndFocus;	// 현재 윈도우가 포커스 되어있는지 여부
MainGame	g_mainGame;
LPSTR		g_lpszClass = (LPSTR)TEXT("Factorio-Imitation");
POINT		g_ptMouse;
float		g_mousezDelta;
glm::vec2	g_cursorPosition;	// 마우스 커서의 포지션
glm::ivec2	g_cursorCoord;		// 마우스 커서의 좌표
glm::vec2	g_currScreenSize;	// 현재 씬 윈도우의 크기
float		g_time;
bool		g_debuggingMode = false;
const ItemSpec	g_itemSpecs[] = {
//		name				buildable	coordSize	direction	passable	fuel,	fuelValue,	stackSize
//										Count
	{	"Assembling Machine 1",		true,		{ 3, 3 },	1,		FALSE,		FALSE,	NULL,		50,	},
	{	"Burner Mining Drill",		true,		{ 2, 2 },	4,		FALSE,		FALSE,	NULL,		50,	},
	{	"Iron Plate",			FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		100,	},
	{	"Coal",				FALSE,		{ 0, 0 },	NULL,		FALSE,		true,	4000,		50,	},
	{	"Transport Belt",		true,		{ 1, 1 },	4,		true,		FALSE,	NULL,		100,	},
	{	"Burner Inserter",		true,		{ 1, 1 },	4,		FALSE,		FALSE,	NULL,		50,	},
	{	"Wood",				FALSE,		{ 0, 0 },	NULL,		FALSE,		true,	2000,		100,	},
	{	"Stone Furnace",		true,		{ 2, 2 },	1,		FALSE,		FALSE,	NULL,		50,	},
	{	"Solid Fuel",			FALSE,		{ 0, 0 },	NULL,		FALSE,		true,	12000,		50,	},
	{	"Rocket Fuel",			FALSE,		{ 0, 0 },	NULL,		FALSE,		true,	100000,		10,	},
	{	"Nuclear Fuel",			FALSE,		{ 0, 0 },	NULL,		FALSE,		true,	1210000,	1,	},
	{	"Iron Ore",			FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		50,	},
	{	"Copper Ore",			FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		50,	},
	{	"Copper Plate",			FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		100,	},
	{	"Stone",			FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		50,	},
	{	"Stone Brick",			FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		100,	},
	{	"Steel Plate",			FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		100,	},
	{	"Wooden Chest",			true,		{ 1, 1 },	1,		FALSE,		FALSE,	NULL,		50,	},
	{	"Fast Transport Belt",		true,		{ 1, 1 },	4,		true,		FALSE,	NULL,		100,	},
	{	"Express Transport Belt",	true,		{ 1, 1 },	4,		true,		FALSE,	NULL,		100,	},
	{	"Iron Gear Wheel",		FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		100,	},
	{	"Copper Cable",			FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		200,	},
	{	"Electronic Circuit",		FALSE,		{ 0, 0 },	NULL,		FALSE,		FALSE,	NULL,		200,	},
	{	"Iron Chest",			true,		{ 1, 1 },	1,		FALSE,		FALSE,	NULL,		50,	},
	{	"Steel Chest",			true,		{ 1, 1 },	1,		FALSE,		FALSE,	NULL,		50,	},
};
// enum ItemEnum;	// 정의 피킹용

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
		if (g_hWnd == GetFocus())
			g_hWndFocus = true;
		else
			g_hWndFocus = false;

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
