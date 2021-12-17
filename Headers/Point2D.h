#ifndef POINT2D_H
#define POINT2D_H

#include "Vector2D.h"
#include <string>

struct Point2D
{
    int x;
    int y;

    Point2D() :
        x(0), y(0)
    {}
    Point2D(int inX, int inY) :
        x(inX), y(inY)
    {}

    bool IsWithinBounds(const int xMax, const int yMax) const
    {
        return(x >= 0 && y >= 0 && x < xMax && y < yMax);
    }
    bool operator==(const Point2D& p) const
    {
        return(x == p.x && y == p.y);
    }
    bool operator!=(const Point2D& p) const
    {
        return(x != p.x || y != p.y);
    }
    bool operator<(const Point2D& p) const
    {
        return(x < p.x || (x == p.x && y < p.y));
    }
    Point2D operator+(const Point2D& p) const
    {
        return(Point2D(x + p.x, y + p.y));
    }
    Point2D operator+(const Vector2D& v) const
    {
        return(Point2D(x + v.x, y + v.y));
    }
    void operator+=(const Vector2D& v)
    {
        x += v.x;
        y += v.y;
    }
    Vector2D operator-(const Point2D& p) const
    {
        return(Vector2D(x - p.x, y - p.y));
    }
    Point2D operator-(const Vector2D& v) const
    {
        return(Point2D(x - v.x, y - v.y));
    }
    void operator-=(const Vector2D& v)
    {
        x -= v.x;
        y -= v.y;
    }
    friend ostream& operator<<(ostream& os, const Point2D& p)
	{
		os << p.ToString();
		return os;
	}
    string ToString() const
    {
        char buffer[256];
        sprintf_s(buffer, "(%d, %d)", x, y);
        return string(buffer);
    }
};

#endif // POINT2D_H