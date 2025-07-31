#include "Vector2.h"
#include <iostream>

// 전역 변수 초기화.
Vector2 Vector2::Zero = Vector2(0, 0);
Vector2 Vector2::One = Vector2(1, 1);
Vector2 Vector2::Up = Vector2(0, 1);
Vector2 Vector2::Right = Vector2(1, 0);

Vector2::Vector2(int x, int y)
    : x(x), y(y)
{
}

Vector2::~Vector2()
{
    if (value == nullptr)
    {
        return;
    }

    delete[] value;
}

Vector2 Vector2::operator+(const Vector2& other) const
{
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
    return Vector2(x - other.x, y - other.y);
}

bool Vector2::operator==(const Vector2& other) const
{
    return x == other.x && y == other.y;
}

Vector2::operator COORD()
{
    COORD coord;
    coord.X = (short)x;
    coord.Y = (short)y;

    return coord;
}

Vector2::operator COORD() const
{
    COORD coord;
    coord.X = (short)x;
    coord.Y = (short)y;

    return coord;
}

const char* Vector2::ToString()
{
    // "(20, 30)" -> 문자열 조합.
    if (value != nullptr)
    {
        delete[] value;
    }

    value = new char[1024];
    sprintf_s(value, 1024, "(%d, %d)", x, y);
    return value;
}