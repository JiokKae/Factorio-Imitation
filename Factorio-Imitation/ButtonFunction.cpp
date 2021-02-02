﻿#include "ButtonFunction.h"
#include "TileMapToolScene.h"

void ButtonFunction::ChangeScene(void* arg)
{
	ChangeSceneArgument* changeSceneArg = (ChangeSceneArgument*)arg;

	SceneManager::GetSingleton()->ChangeScene(changeSceneArg->sceneName.c_str(), changeSceneArg->loadingSceneName.c_str());
}

void ButtonFunction::QuitProgram(void* arg)
{
	if (MessageBox(g_hWnd, "게임을 종료하시겠습니까?", "종료 확인", MB_OKCANCEL) == IDOK)
	{
		PostQuitMessage(0);
	}
}

void ButtonFunction::TileInfoSave(void* arg)
{
	TileInfoArgument* tileInfoArg = (TileInfoArgument*)arg;

	DWORD writtenByte;
	HANDLE hFile = CreateFile("Save/saveMapData.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, tileInfoArg->tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &writtenByte, NULL);

	CloseHandle(hFile);
}

void ButtonFunction::TileInfoLoad(void* arg)
{
	TileInfoArgument* tileInfoArg = (TileInfoArgument*)arg;

	DWORD readByte;
	HANDLE hFile = CreateFile("Save/saveMapData.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(hFile, tileInfoArg->tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &readByte, NULL);

	CloseHandle(hFile);
}
