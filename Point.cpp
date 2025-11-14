#include "Point.h"

namespace SimplyGo
{
    SimplyGo::Point::Point(int x_cord, int y_cord)
    {
        x = x_cord;
        y = y_cord;
    }

    Point SimplyGo::Point::operator+(const Point &n)
    {
        return Point(x + n.x, y + n.y);
    }

    Point SimplyGo::Point::operator-(const Point &n)
    {
        return Point(x - n.x, y - n.y);
    }

    Point SimplyGo::Point::operator*(const int &n)
    {
        return Point(x * n, y * n);
    }

    Point SimplyGo::Point::operator/(const int &n)
    {
        return Point(x / n, y / n);
    }

    bool Point::operator==(const Point &other)
    {
        return false;
    }
}