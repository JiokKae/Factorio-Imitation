﻿#include "framework.h"

bool Vec2::operator<(const Vec2& vec) const
{
	if (y != vec.y)
	{
		return y < vec.y;
	}
	return x > vec.x;
}

bool Vec2::operator>(const Vec2& vec) const
{
	if (y != vec.y)
	{
		return y > vec.y;
	}
	return x < vec.x;
}

bool ItemInfo::IsEmpty() const
{
	return amount == 0;
}

void ItemInfo::IncreaseAmount(int value)
{
	amount += value;
	if (amount < 0)
		amount = 0;
}

void ItemInfo::SetAbleItems(const std::vector<int>& _vecAbleItems)
{
	vecAbleItems = _vecAbleItems;
}

void ItemInfo::AddAbleItem(int itemEnum)
{
	vecAbleItems.push_back(itemEnum);
}

bool ItemInfo::CanInput(int itemId) const
{
	// 무제한 슬롯일 때
	if (vecAbleItems.size() == 0)
	{
		return true;
	}

	// 제한있는 슬롯일 때
	for (int i = 0; i < vecAbleItems.size(); ++i)
	{
		// 허용된 아이템이라면
		if (itemId == vecAbleItems[i])
			return true;
	}

	return false;
}

bool ItemInfo::MoveAllItemTo(ItemInfo* destItemInfo)
{
	if (destItemInfo->CanInput(id) == false)
	{
		return false;
	}

	if (destItemInfo->IsEmpty() == true)
	{
		destItemInfo->id = id;
		destItemInfo->amount = amount;
		amount = 0;
		return true;
	}

	if (destItemInfo->id == id)
	{
		destItemInfo->amount += amount;
		amount = 0;
		return true;
	}

	return false;
}

void ItemInfo::SwapItemWith(ItemInfo* destItemInfo)
{
	ItemInfo temp(id, amount);
	id = destItemInfo->id;
	amount = destItemInfo->amount;
	destItemInfo->id = temp.id;
	destItemInfo->amount = temp.amount;
}

ItemInfo::ItemInfo(int id /*= 0*/, int amount /*= 0*/)
	: id{ id }
	, amount{ amount }
{
}

bool ItemInfo::operator>(const ItemInfo& info) const
{
	if (id != info.id)
		return id > info.id;
	else
		return amount > info.amount;
}

bool ItemInfo::operator<(const ItemInfo& info) const
{
	if (id != info.id)
		return id < info.id;
	else
		return amount < info.amount;
}
