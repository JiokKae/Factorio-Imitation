#pragma once
#include "Structure.h"

class Container : public Structure
{
protected:
	int inventorySize;
	vector<ItemInfo> vecInventory;
public:
	// Need to set inventorySize, ItemId
	HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release() override;

	// 아이템을 컨테이너에 넣는 함수
	virtual bool InputItem(ItemInfo* item, glm::vec2 pos) override;

	//virtual bool TakeOutItem(ItemInfo* outItem) override { return false; };

	virtual string ToString() override;
};

