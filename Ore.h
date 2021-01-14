#pragma once
#include "GameNode.h"
#include "ItemEnum.h"
class Ore : public GameNode
{
private:
	int itemEnum;
	int amount;
	int randFrameX;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();

	int GetItemEnum()		{ return itemEnum; }
	int GetAmount()			{ return amount; }
	int GetRandFrameX()		{ return randFrameX; }

	void AddAmount(int value) { this->amount += value; }

	int AmountToImageFrameY();

};

