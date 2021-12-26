#ifndef POINT3D_H
#define POINT3D_H

#include "Vector3D.h"
#include <string>
#include <inttypes.h>

struct Point3D
{
    int64_t x;
    int64_t y;
    int64_t z;

    Point3D() :
        x(0), y(0), z(0)
    {}
    Point3D(int64_t inX, int64_t inY, int64_t inZ) :
        x(inX), y(inY), z(inZ)
    {}

    bool operator==(const Point3D& p) const
    {
        return(x == p.x && y == p.y && z == p.z);
    }
    bool operator!=(const Point3D& p) const
    {
        return(x != p.x || y != p.y || z != p.z);
    }
    bool operator<(const Point3D& p) const
    {
        return(x < p.x || (x == p.x && y < p.y) || (x == p.x && y == p.y && z < p.z));
    }
    Point3D operator+(const Point3D& p) const
    {
        return(Point3D(x + p.x, y + p.y, z + p.z));
    }
    Point3D operator+(const Vector3D& v) const
    {
        return(Point3D(x + v.x, y + v.y, z + v.z));
    }
    void operator+=(const Vector3D& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    Vector3D operator-(const Point3D& p) const
    {
        return(Vector3D(x - p.x, y - p.y, z - p.z));
    }
    Point3D operator-(const Vector3D& v) const
    {
        return(Point3D(x - v.x, y - v.y, z - v.z));
    }
    void operator-=(const Vector3D& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    friend ostream& operator<<(ostream& os, const Point3D& p)
	{
		os << p.ToString();
		return os;
	}
    string ToString() const
    {
        char buffer[256];
        sprintf_s(buffer, "(%" PRId64 ", %" PRId64 " , %" PRId64 ")", x, y, z);
        return string(buffer);
    }
};

#endif // POINT3D_H