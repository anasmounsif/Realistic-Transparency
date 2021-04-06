#pragma once
#include "vector3D.h"

struct point3D
{
	float		x, y, z;

	point3D() = default;

	point3D(float a, float b, float c) { x = a; y = b; z = c; }

	float& operator [](int i)
	{
		return ((&x)[i]);
	}

	const float& operator [](int i) const
	{
		return ((&x)[i]);
	}
};

inline vector3D operator -(const point3D& a, const point3D& b)
{
	return (vector3D(a.x - b.x, a.y - b.y, a.z - b.z));
}

inline point3D operator +(const point3D& a, const vector3D& b)
{
	return (point3D(a.x + b.x, a.y + b.y, a.z + b.z));
}

inline point3D operator +(const vector3D& b, const point3D& a)
{
	return (point3D(a.x + b.x, a.y + b.y, a.z + b.z));
}

inline point3D operator -(const point3D& a, const vector3D& b)
{
	return (point3D(a.x - b.x, a.y - b.y, a.z - b.z));
}

inline point3D operator -(const vector3D& a, const point3D& b)
{
	return (point3D(a.x - b.x, a.y - b.y, a.z - b.z));
}
