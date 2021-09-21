#pragma once
#include "GameNode.h"
#include "ItemEnum.h"
class Ore : public GameNode
{
private:
	int itemEnum;
	int amount;
	int randFrameX;
	int frameYByAmount;

public:
	virtual HRESULT Init(int x, int y);
	virtual void Release();
	virtual void Update();

	int GetItemEnum()		{ return itemEnum; }
	int GetAmount()			{ return amount; }
	int GetRandFrameX()		{ return randFrameX; }
	int GetFrameY()			{ return frameYByAmount; }
	void AddAmount(int value); 

	int AmountToImageFrameY();

};

