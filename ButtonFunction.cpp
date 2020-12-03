#include "ButtonFunction.h"
#include "TileMapToolScene.h"

void ButtonFunction::ChangeScene(void* arg)
{
	Argument* argu = (Argument*)arg;

	SceneManager::GetSingleton()->ChangeScene(argu->a.c_str(), argu->b.c_str());
}

void ButtonFunction::QuitProgram(void* arg)
{
	//int* point = (int*)arg;
	//if (MessageBox(g_hWnd, string("게임을 종료하시겠습니까?" + to_string(*point)).c_str(), "종료 확인", MB_OKCANCEL) == IDOK)
	if (MessageBox(g_hWnd, "게임을 종료하시겠습니까?", "종료 확인", MB_OKCANCEL) == IDOK)
	{
		PostQuitMessage(0);
	}
}

void ButtonFunction::TileInfoSave(void* arg)
{
	TILE_INFO* tileInfo = (TILE_INFO*)arg;

	DWORD writtenByte;
	HANDLE hFile = CreateFile("Save/saveMapData.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &writtenByte, NULL);

	CloseHandle(hFile);
}

void ButtonFunction::TileInfoLoad(void* arg)
{
	TILE_INFO* tileInfo = (TILE_INFO*)arg;

	DWORD readByte;
	HANDLE hFile = CreateFile("Save/saveMapData.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(hFile, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &readByte, NULL);

	CloseHandle(hFile);
}
