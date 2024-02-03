#include "framework.h"

FRECT FRECT::operator+(const FRECT& rect) const
{
	return FRECT{
		left + rect.left,
		top + rect.top,
		right + rect.right,
		bottom + rect.bottom,
	};
}

FRECT FRECT::operator*(float scalar) const
{
	return FRECT{
		left * scalar,
		top * scalar,
		right * scalar,
		bottom * scalar,
	};
}
