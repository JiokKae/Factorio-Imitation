#pragma once
#include "GameNode.h"

#define	TILE_SIZE		32
#define	SAMPLE_TILE_X	20
#define	SAMPLE_TILE_Y	9

#define	TILE_X			20
#define	TILE_Y			20

enum TERRAIN { WALL, GRASS, EARTH, WATER, CEMENT, END };

typedef struct tagTile
{
	TERRAIN terrian;
	RECT rc;
	int frameX;
	int frameY;
} TILE_INFO;

typedef struct tagSampleTile
{
	RECT rc;
	int frameX;
	int frameY;
} SAMPLE_TILE_INFO;

typedef struct tagSelectTile 
{
	RECT frame;
} SELECT_TILE_INFO;

class Image;
class Button;
class TileMapToolScene : public GameNode
{
private:
	Image* sampleTile;
	Image* selectTileEdge;
	// ���� Ÿ���� ����
	RECT rcMain;
	TILE_INFO tileInfo[TILE_X * TILE_Y];

	// ���� Ÿ���� ����
	RECT rcSample;
	SAMPLE_TILE_INFO sampleTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];

	// ���õ� Ÿ���� ����
	SELECT_TILE_INFO selectTileInfo;
	POINT selectStartFrame;
	bool sampleSelectStart;


	// ���̺� �ε� ��ư
	Button* saveButton;
	Button* loadButton;
public:
	virtual HRESULT Init()		 override;
	virtual void Release()		 override;
	virtual void Update()		 override;
	virtual void Render(HDC hdc) override;

	TileMapToolScene() : sampleSelectStart(false) {};
	virtual ~TileMapToolScene() {};
};

