#ifndef POINT3D_H
#define POINT3D_H

#include "Vector3D.h"
#include <string>

struct Point3D
{
    int x;
    int y;
    int z;

    Point3D() :
        x(0), y(0), z(0)
    {}
    Point3D(int inX, int inY, int inZ) :
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
        sprintf_s(buffer, "(%d, %d, %d)", x, y, z);
        return string(buffer);
    }
};

#endif // POINT3D_H