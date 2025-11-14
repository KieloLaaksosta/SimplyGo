#ifndef POINT_H
#define POINT_H

namespace SimplyGo
{
    struct Point
    {
        int x;
        int y;

        Point(int x_cord, int y_cord);

        Point operator+(const Point &n);
        Point operator-(const Point &n);

        Point operator*(const int &n);
        Point operator/(const int &n);

        bool operator==(const Point &other);
    };

    extern const Point DIRECTIONS[];
}

#endif