#include "Geometry.h"
#include <cmath>

void Vector2::operator+=(const Vector2& rval)
{
	x += rval.x;
	y += rval.y;
}

void Vector2::operator-=(const Vector2& rval)
{
	x -= rval.x;
	y -= rval.y;
}

Vector2 Vector2::operator*(float scale) const
{
	return {x * scale, y * scale};
}

Vector2 Vector2::operator/(float div) const
{
	return {x / div, y / div};
}

void Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

void Vector2::operator/=(float div)
{
	x /= div;
	y /= div;
}

Vector2 Vector2::operator-()const 
{
	return { -x, -y };
}

float Vector2::Length() const
{
	return hypotf(x, y);
}

float Vector2::SQLength() const
{
	return x * x + y * y;
}

void Vector2::Normalize()
{
	auto len = Length();
	x /= len;
	y /= len;
}

Vector2 Vector2::Normalized() const
{
	auto len = Length();
	return { x / len, y / len };
}
