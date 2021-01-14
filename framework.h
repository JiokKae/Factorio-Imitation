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
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

struct Vec2 : public glm::vec2
{
	Vec2() : glm::vec2() {};
	template <typename T>
	Vec2(T const& s1, T const& s2) : glm::vec2(s1, s2) { };
	Vec2(glm::vec2 v)				{ this->x = v.x; this->y = v.y; };
	bool operator<(const Vec2& vec) const;
	bool operator>(const Vec2& vec) const;
};

typedef struct tagFRECT
{
	float left;
	float top;
	float right;
	float bottom;
} FRECT, * LPFRECT;

struct ItemInfo {
	int id;
	int amount;
	vector<int> vecAbleItems;

	void AddAmount(int amount)
	{
		this->amount += amount;
		if (this->amount < 0)
			this->amount = 0;
	}
	void SetAbleItems(vector<int> vecAbleItems)
	{
		this->vecAbleItems = vecAbleItems;
	}

	void AddAbleItem(int itemEnum)
	{
		vecAbleItems.push_back(itemEnum);
	}

	bool InputItem(ItemInfo* itemInfo)
	{
		// 무제한 슬롯일 때
		if (vecAbleItems.size() == 0)
		{
			// 같은 아이템이라면
			if (itemInfo->amount && itemInfo->id == id)
			{
				amount += itemInfo->amount;
				itemInfo->amount = 0;
			}
			// 다른 아이템이라면
			else
			{
				ItemInfo temp(id, amount);
				id = itemInfo->id;
				amount = itemInfo->amount;
				itemInfo->id = temp.id;
				itemInfo->amount = temp.amount;
			}
			return true;
		}
		// 제한있는 슬롯일 때
		else
		{
			// 들어온 아이템이 있을 때
			if (itemInfo->amount)
			{
				for (int i = 0; i < vecAbleItems.size(); i++)
				{
					// 허용된 아이템이라면
					if (itemInfo->id == vecAbleItems[i])
					{
						// 같은 아이템이라면
						if (itemInfo->amount && itemInfo->id == id)
						{
							amount += itemInfo->amount;
							itemInfo->amount = 0;
						}
						// 다른 아이템이라면
						else
						{
							ItemInfo temp(id, amount);
							id = itemInfo->id;
							amount = itemInfo->amount;
							itemInfo->id = temp.id;
							itemInfo->amount = temp.amount;
						}
						return true;
					}
				}
			}
			// 들어온 아이템이 없을 때
			else
			{
				ItemInfo temp(id, amount);
				amount = itemInfo->amount;
				itemInfo->id = temp.id;
				itemInfo->amount = temp.amount;
				return true;
			}
		}
		// 허용되지 않은 아이템 처리
		return false;
	}

	ItemInfo(int id = 0, int amount = 0)
	{
		this->id = id;
		this->amount = amount;
	}

};

// Singletons
#include "KeyManager.h"
#include "TimerManager.h"
#include "ImageManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "TextRenderer.h"
#include "UIManager.h"
#include "TileManager.h"
#include "EntityManager.h"
#include "RecipeManager.h"

// Useful
#include "GLImage.h"
#include "Shader.h"

#define WINSIZE_X	900
#define WINSIZE_Y	900
#define WINSIZE_TITLE_X		1600
#define WINSIZE_TITLE_Y		900
#define WINSIZE_TILE_MAP_X	1600
#define WINSIZE_TILE_MAP_Y	900
#define SAFE_DELETE(p) 				{ if (p) delete p, p = nullptr; }
#define SAFE_ARR_DELETE(p) 			{ if (p) delete[] p, p = nullptr; }
#define SAFE_RELEASE(p) 			{ if (p) p->Release(), delete p, p = nullptr; }
#define SAFE_ARR_RELEASE(p, size)	{ if (p)	{ for (int i = 0; i < size; i++) { p[i].Release(); } delete[] p, p = nullptr; } }

#define TILE_SIZE		64
#define CHUNK_IN_TILE	32

#define POSX_TO_COORDX(x)				( (x < 0)? int(x) / TILE_SIZE - 1 : int(x) / TILE_SIZE )
#define POS_TO_COORD(pos)				{ POSX_TO_COORDX(pos.x), POSX_TO_COORDX(pos.y) }
#define COORDX_TO_POSX(x)				( x * TILE_SIZE + TILE_SIZE / 2 )
#define COORD_TO_POS(coord)				{ COORDX_TO_POSX(coord.x), COORDX_TO_POSX(coord.y) }
#define TILECOORDX_TO_CHUNKCOORDX(x)	( (x < 0)? int(x) / CHUNK_IN_TILE - 1 : int(x) / CHUNK_IN_TILE )
#define TILECOORD_TO_CHUNKCOORD(coord)	{ TILECOORDX_TO_CHUNKCOORDX(coord.x), TILECOORDX_TO_CHUNKCOORDX(coord.y) }

// Inventory
#define SLOT_X	10
#define SLOT_Y	9
#define SLOT_SIZE	(SLOT_X * SLOT_Y)

struct ItemSpec {
	string name;
	bool buildable;
	glm::ivec2 coordSize;
	int directionCount;
	bool passable;
	bool fuel;
	float fuelValue;
};

enum DIRECTION {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	DIRECTION_END
};

#define OPPOSITE_DIR(x) ((x + 2) % DIRECTION_END)
#define LEFT_DIR(x)		((x + 3) % DIRECTION_END)
#define RIGHT_DIR(x)	((x + 1) % DIRECTION_END)

extern HWND			g_hWnd;
extern float		g_time;
extern HINSTANCE	g_hInstance;
extern POINT		g_ptMouse;
extern float		g_mousezDelta;
extern glm::vec2	g_cursorPosition;	// 마우스 커서의 포지션
extern glm::vec2	g_cursorCoord;		// 마우스 커서의 좌표
extern glm::vec2	g_currScreenSize;	// 현재 씬 윈도우의 크기
extern ItemSpec		g_itemSpecs[];
extern const char*	g_directionToLpChar[];

enum class Argument_Kind {
	None,
	ChangeSceneArgument,
	TileInfoArgument,
};

struct ChangeSceneArgument
{
	string sceneName;
	string loadingSceneName;

	ChangeSceneArgument(string sceneName, string loadingSceneName)
	{
		this->sceneName = sceneName;
		this->loadingSceneName = loadingSceneName;
	}
};

typedef struct tagTile TILE_INFO;
struct TileInfoArgument
{
	TILE_INFO* tileInfo;

	TileInfoArgument(TILE_INFO* lpTileInfo)
	{
		this->tileInfo = lpTileInfo;
	}
};


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

inline bool IntersectFRect(LPFRECT lpfrcDst, const FRECT* lpfrcSrc1, const FRECT* lpfrcSrc2)
{
	if (lpfrcSrc1->right	< lpfrcSrc2->left ||
		lpfrcSrc1->left		> lpfrcSrc2->right ||
		lpfrcSrc1->top		< lpfrcSrc2->bottom ||
		lpfrcSrc1->bottom	> lpfrcSrc2->top)
	{
		return false;
	}
	else
	{
		*lpfrcDst = *lpfrcSrc1;
		if (lpfrcSrc2->left > lpfrcSrc1->left && lpfrcSrc2->left <= lpfrcSrc1->right)
			lpfrcDst->left = lpfrcSrc2->left;

		if (lpfrcSrc2->right >= lpfrcSrc1->left && lpfrcSrc2->right < lpfrcSrc1->right)
			lpfrcDst->right = lpfrcSrc2->right;

		if (lpfrcSrc2->top >= lpfrcSrc1->bottom && lpfrcSrc2->top < lpfrcSrc1->top)
			lpfrcDst->top = lpfrcSrc2->top;

		if (lpfrcSrc2->bottom > lpfrcSrc1->bottom && lpfrcSrc2->bottom <= lpfrcSrc1->top)
			lpfrcDst->bottom = lpfrcSrc2->bottom;
	}
	

	return true;
}

inline bool CheckRectCollision(FRECT rc1, FRECT rc2)
{
	if (rc1.right < rc2.left ||
		rc1.left > rc2.right ||
		rc1.top < rc2.bottom ||
		rc1.bottom > rc2.top)
	{
		return false;
	}

	return true;
}

inline bool CheckRectCollision(RECT rc1, RECT rc2)
{
	if (rc1.right < rc2.left ||
		rc1.left > rc2.right ||
		rc1.top < rc2.bottom ||
		rc1.bottom > rc2.top)
	{
		return false;
	}

	return true;
}

inline bool PtInRect(RECT rc, glm::vec2 pt)
{
	if (rc.right < pt.x ||
		rc.left > pt.x ||
		rc.top < pt.y ||
		rc.bottom > pt.y)
	{
		return false;
	}

	return true;
}

inline bool PtInFRect(FRECT rc, glm::vec2 pt)
{
	if (rc.right < pt.x ||
		rc.left > pt.x ||
		rc.top < pt.y ||
		rc.bottom > pt.y)
	{
		return false;
	}

	return true;
}
