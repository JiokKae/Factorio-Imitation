#include "framework.h"

bool Vec2::operator<(const Vec2& vec) const
{
    if (y != vec.y)
    {
        return y < vec.y;
    }
    else
        return x > vec.x;
}

bool Vec2::operator>(const Vec2& vec) const
{
    if (y != vec.y)
    {
        return y > vec.y;
    }
    else
        return x < vec.x;
}

bool ItemInfo::IsEmpty()
{
    return amount == 0;
}

void ItemInfo::SetAbleItems(vector<int> _vecAbleItems)
{
	vecAbleItems = _vecAbleItems;
}

void ItemInfo::AddAbleItem(int itemEnum)
{
	vecAbleItems.push_back(itemEnum);
}

bool ItemInfo::CanInput(int itemId)
{
    // 무제한 슬롯일 때
    if (vecAbleItems.size() == 0)
        return true;

    // 제한있는 슬롯일 때
    else
    {
        for (int i = 0; i < vecAbleItems.size(); i++)
        {
            // 허용된 아이템이라면
            if (itemId == vecAbleItems[i])
                return true;
        }
    }

    return false;
}

bool ItemInfo::MoveAllItemTo(ItemInfo* destItemInfo)
{
    // 목적지가 비어있거나 같은 종류라면 이동 가능
    if (destItemInfo->IsEmpty())
    {
        destItemInfo->id = id;
        destItemInfo->amount = amount;
        amount = 0;
        return true;
    }
    else if (destItemInfo->id == id)
    {
        destItemInfo->amount += amount;
        amount = 0;
        return true;
    }
    else
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

tagFRECT::tagFRECT(float left, float top, float right, float bottom)
{
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
}

const tagFRECT tagFRECT::operator+(const tagFRECT& rect) const
{
    tagFRECT ret;
    ret.left = left + rect.left;
    ret.top = top + rect.top;
    ret.right = right + rect.right;
    ret.bottom = bottom + rect.bottom;

    return ret;
}
