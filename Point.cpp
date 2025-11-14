#include "Point.h"

const SimplyGo::Point SimplyGo::DIRECTIONS[] = {SimplyGo::Point(-1, 0), SimplyGo::Point(1, 0), SimplyGo::Point(0, -1), SimplyGo::Point(0, 1)};

SimplyGo::Point::Point(int x_cord, int y_cord)
{
    x = x_cord;
    y = y_cord;
}

SimplyGo::Point SimplyGo::Point::operator+(const Point &n)
{
    return Point(x + n.x, y + n.y);
}

SimplyGo::Point SimplyGo::Point::operator-(const Point &n)
{
    return Point(x - n.x, y - n.y);
}

SimplyGo::Point SimplyGo::Point::operator*(const int &n)
{
    return Point(x * n, y * n);
}

SimplyGo::Point SimplyGo::Point::operator/(const int &n)
{
    return Point(x / n, y / n);
}

bool SimplyGo::Point::operator==(const Point &other)
{
    return false;
}