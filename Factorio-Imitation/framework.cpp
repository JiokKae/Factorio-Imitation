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
