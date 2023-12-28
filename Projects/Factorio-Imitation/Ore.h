#pragma once

class Ore
{
public:
	Ore(int itemEnum, int amount);

	int GetItemEnum() const;
	int GetAmount() const;
	int GetRandFrameX() const;
	int GetFrameY() const;
	
	void AddAmount(int value);

	static int RandomAmount(int x, int y);
	static int RandomItemEnum(int x, int y);
private:
	int itemEnum;
	int amount;
	int randFrameX;
};
