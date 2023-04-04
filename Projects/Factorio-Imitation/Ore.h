#pragma once

class Ore
{
public:
	Ore(int x, int y);

	int GetItemEnum() const;
	int GetAmount() const;
	int GetRandFrameX() const;
	int GetFrameY() const;
	
	void AddAmount(int value);

private:
	int itemEnum;
	int amount;
	int randFrameX;

	int RandomAmount(int x, int y) const;
	int RandomItemEnum(int x, int y) const;
};
