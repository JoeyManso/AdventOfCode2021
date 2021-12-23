#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "AdventMath.h"
#include "Point3D.h"

class Vector3D {
public:
	int x;
	int y;
	int z;
	Vector3D() :
		x(0), y(0), z(0)
	{}
	Vector3D(int inX, int inY, int inZ) :
		x(inX), y(inY), z(inZ)
	{}
	Vector3D(const Vector3D& v) :
		x(v.x), y(v.y), z(v.z)
	{}

	Vector3D operator-(const Vector3D& v) const
	{
		return(Vector3D(x - v.x, y - v.y, z - v.z));
	}

	Vector3D operator+(const Vector3D& v) const
	{
		return(Vector3D(x + v.x, y + v.y, z + v.z));
	}

	bool operator<(const Vector3D& v) const
    {
        return(x < v.x || (x == v.x && y < v.y) || (x == v.x && y == v.y && z < v.z));
    }

	Vector3D& operator-=(const Vector3D& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return(*this);
	}

	Vector3D& operator+=(const Vector3D& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return(*this);
	}

	Vector3D operator*(const int magnitude) const
	{
		return(Vector3D(x * magnitude, y * magnitude, z * magnitude));
	}

	bool operator==(const Vector3D& v) const
	{
		return(x == v.x && y == v.y && z == v.z);
	}

	bool operator!=(const Vector3D& v) const
	{
		return(x != v.x || y != v.y || z != v.z);
	}

	friend ostream& operator<<(ostream& os, const Vector3D& v)
	{
		os << v.ToString();
		return os;
	}

	float Length() const
	{
		return sqrt(float(sqr(x) + sqr(y) + sqr(z)));
	}

	void Normalize()
	{
		float l = Length();
		float newX = static_cast<float>(x) / l;
		float newY = static_cast<float>(y) / l;
		float newZ = static_cast<float>(z) / l;
		x = static_cast<int>(round(newX));
		y = static_cast<int>(round(newY));
		z = static_cast<int>(round(newZ));
	}

	static float Dist(const Vector3D& v1, const Vector3D& v2)
	{
		return(v1 - v2).Length();
	}

	static int Manhattan(const Vector3D& v1, const Vector3D& v2)
	{
		return abs(v1.x - v2.x) + abs(v1.y - v2.y) + abs(v1.z - v2.z);
	}

	string ToString() const
    {
        char buffer[256];
        sprintf_s(buffer, "(%d, %d, %d)", x, y, z);
        return string(buffer);
    }
};

static Vector3D operator*(const float magnitude, const Vector3D v)
{
	return(Vector3D(int(float(v.x) * magnitude), int(float(v.y) * magnitude), int(float(v.z) * magnitude)));
};

static Vector3D operator*(const Vector3D v, const float magnitude)
{
	return(Vector3D(int(float(v.x) * magnitude), int(float(v.y) * magnitude), int(float(v.z) * magnitude)));
};

#endif // VECTOR2D_H