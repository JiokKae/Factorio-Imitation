#include "TileMapToolScene.h"
#include "Image.h"
#include "Button.h"
#include "ButtonFunction.h"

HRESULT TileMapToolScene::Init()
{
	SetWindowSize(50, 50, WINSIZE_TILE_MAP_X, WINSIZE_TILE_MAP_Y);

	// 이미지 등록
	sampleTile = ImageManager::GetSingleton()->AddImage("SampleTile", "Image/maptiles.bmp", 640, 288, SAMPLE_TILE_X, SAMPLE_TILE_Y);
	selectTileEdge = ImageManager::GetSingleton()->AddImage("SelectTileEdge", "Image/selectTileEdge.bmp", 64, 64, 2, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("SaveLoadButton", "Image/button2.bmp", 300, 140, 2, 2);

	// 세이브, 로드 버튼
	saveButton = new Button();
	saveButton->Init("SaveLoadButton", WINSIZE_TILE_MAP_X - sampleTile->GetWidth(), sampleTile->GetHeight() + 400, {0, 0}, {0, 1});
	saveButton->SetButtonFunc(ButtonFunction::TileInfoSave, tileInfo);

	loadButton = new Button();
	loadButton->Init("SaveLoadButton", WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + 200, sampleTile->GetHeight() + 400, { 1, 0 }, { 1, 1 });
	loadButton->SetButtonFunc(ButtonFunction::TileInfoLoad, tileInfo);

	// 셀렉트 영역 초기화
	selectTileInfo.frame.left = 0;
	selectTileInfo.frame.right = 0;
	selectTileInfo.frame.top = 0;
	selectTileInfo.frame.bottom = 0;

	// 오른쪽 상단에 샘플 타일의 정보를 세팅
	
	rcSample.left = WINSIZE_TILE_MAP_X - sampleTile->GetWidth() - 50;
	rcSample.right = WINSIZE_TILE_MAP_X - 50;
	rcSample.top = 50;
	rcSample.bottom = 50 + sampleTile->GetHeight();
	for (int i = 0; i < SAMPLE_TILE_Y; i++)
	{
		for (int j = 0; j < SAMPLE_TILE_X; j++)
		{
			SetRect(&sampleTileInfo[i * TILE_X + j].rc, 
				WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + j * TILE_SIZE,
				WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + i * TILE_SIZE,
				WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + j * TILE_SIZE + TILE_SIZE,
				WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + i * TILE_SIZE + TILE_SIZE);

			sampleTileInfo[i * TILE_X + j].frameX = j;
			sampleTileInfo[i * TILE_X + j].frameY = i;
		}
	}

	// 왼쪽 상단에 메인 타일의 정보를 세팅
	rcMain.left = 0;
	rcMain.top = 0;
	rcMain.right = rcMain.left + TILE_X * TILE_SIZE;
	rcMain.bottom = rcMain.top + TILE_X * TILE_SIZE;
	for (int i = 0; i < TILE_Y; i++)
	{
		for (int j = 0; j < TILE_X; j++)
		{
			SetRect(&tileInfo[i * TILE_X + j].rc, 
				j * TILE_SIZE, 
				i * TILE_SIZE, 
				j * TILE_SIZE + TILE_SIZE, 
				i * TILE_SIZE + TILE_SIZE);

			tileInfo[i * TILE_X + j].terrian = TERRAIN::GRASS;
			tileInfo[i * TILE_X + j].frameX = 0;
			tileInfo[i * TILE_X + j].frameY = 1;
		}
	}

	return S_OK;
}

void TileMapToolScene::Release()
{
}

void TileMapToolScene::Update()
{
	#pragma region 선택 영역
	// 샘플 타일 영역 안 인지 확인
	if (sampleSelectStart)
	{
		g_ptMouse.x = Clamp(g_ptMouse.x, rcSample.left, rcSample.right);
		g_ptMouse.y = Clamp(g_ptMouse.y, rcSample.top, rcSample.bottom - 1);
	}

	if (PtInRect(&rcSample, g_ptMouse))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x - rcSample.left;
			int posY = g_ptMouse.y - rcSample.top;
			int idX = posX / TILE_SIZE;
			int idY = posY / TILE_SIZE;
			
			selectTileInfo.frame.left = idX;
			selectTileInfo.frame.right = idX;
			selectTileInfo.frame.top = idY;
			selectTileInfo.frame.bottom = idY;

			selectStartFrame = { idX, idY };
			sampleSelectStart = true;
		}

		if (sampleSelectStart)
		{
			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
			{
				int posX = g_ptMouse.x - rcSample.left;
				int posY = g_ptMouse.y - rcSample.top;
				int idX = posX / TILE_SIZE;
				int idY = posY / TILE_SIZE;

				if (selectStartFrame.x <= idX)
				{
					selectTileInfo.frame.left = selectStartFrame.x;
					selectTileInfo.frame.right = idX;
				}
				else
				{
					selectTileInfo.frame.right = selectStartFrame.x;
					selectTileInfo.frame.left = idX;
				}
				if (selectStartFrame.y <= idY)
				{
					selectTileInfo.frame.top = selectStartFrame.y;
					selectTileInfo.frame.bottom = idY;
				}
				else
				{
					selectTileInfo.frame.bottom = selectStartFrame.y;
					selectTileInfo.frame.top = idY;
				}
			}
		}

		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
		{
			sampleSelectStart = false;
		}
	}
	#pragma endregion

	#pragma region 메인 영역
	if (PtInRect(&rcMain, g_ptMouse))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
		{
			int posX = g_ptMouse.x - rcMain.left;
			int posY = g_ptMouse.y - rcMain.top;
			int idX = posX / TILE_SIZE;
			int idY = posY / TILE_SIZE;

			for (int i = 0; i <= selectTileInfo.frame.bottom - selectTileInfo.frame.top; i++)
			{
				for (int j = 0; j <= selectTileInfo.frame.right - selectTileInfo.frame.left; j++)
				{
					//메인 영역 안에 있어야 복사
					if (idX + j > TILE_X - 1)
						continue;

					if ((idX + j) + (idY + i) * SAMPLE_TILE_X < TILE_X * TILE_Y)
					{
						tileInfo[(idX + j) + (idY + i) * SAMPLE_TILE_X].frameX = j + selectTileInfo.frame.left;
						tileInfo[(idX + j) + (idY + i) * SAMPLE_TILE_X].frameY = i + selectTileInfo.frame.top;
					}
				}
			}
		}
	}
	#pragma endregion

	
	saveButton->Update();
	loadButton->Update();
}

void TileMapToolScene::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, WINSIZE_TILE_MAP_X, WINSIZE_TILE_MAP_Y, WHITENESS);

	
	if (sampleTile)
	{
		// 샘플 타일	
		sampleTile->Render(hdc, rcSample.left, rcSample.top, 640, 288);

		// 메인 타일
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			sampleTile->FrameRender(hdc, tileInfo[i].rc.left + (TILE_SIZE / 2), tileInfo[i].rc.top + (TILE_SIZE / 2), tileInfo[i].frameX, tileInfo[i].frameY);
		}

		// 선택된 타일들
		for (int i = 0; i <= selectTileInfo.frame.bottom - selectTileInfo.frame.top; i++)
		{
			for (int j = 0; j <= selectTileInfo.frame.right - selectTileInfo.frame.left; j++)
			{
				sampleTile->FrameRender(hdc,
					WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + (j * TILE_SIZE),
					sampleTile->GetHeight() + 100 + (i * TILE_SIZE), 
					j + selectTileInfo.frame.left, i + selectTileInfo.frame.top);
			}
		}
		selectTileEdge->FrameRender(hdc, rcSample.left + (selectTileInfo.frame.left * TILE_SIZE) + TILE_SIZE / 2,
			rcSample.top + (selectTileInfo.frame.top * TILE_SIZE) + TILE_SIZE / 2, 0, 0);
		selectTileEdge->FrameRender(hdc, rcSample.left + (selectTileInfo.frame.right * TILE_SIZE) + TILE_SIZE / 2,
			rcSample.top + (selectTileInfo.frame.top * TILE_SIZE) + TILE_SIZE / 2, 1, 0);
		selectTileEdge->FrameRender(hdc, rcSample.left + (selectTileInfo.frame.left * TILE_SIZE) + TILE_SIZE / 2,
			rcSample.top + (selectTileInfo.frame.bottom * TILE_SIZE) + TILE_SIZE / 2, 0, 1);
		selectTileEdge->FrameRender(hdc, rcSample.left + (selectTileInfo.frame.right * TILE_SIZE) + TILE_SIZE / 2,
			rcSample.top + (selectTileInfo.frame.bottom * TILE_SIZE) + TILE_SIZE / 2, 1, 1);
	}
	saveButton->Render(hdc);
	loadButton->Render(hdc);
}

