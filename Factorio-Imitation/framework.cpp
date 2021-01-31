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

bool ItemInfo::CanInput(int itemId)
{
    // ������ ������ ��
    if (vecAbleItems.size() == 0)
        return true;

    // �����ִ� ������ ��
    else
    {
        for (int i = 0; i < vecAbleItems.size(); i++)
        {
            // ���� �������̶��
            if (itemId == vecAbleItems[i])
                return true;
        }
    }

    return false;
}

bool ItemInfo::MoveAllItemTo(ItemInfo* destItemInfo)
{
    // �������� ����ְų� ���� ������� �̵� ����
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
