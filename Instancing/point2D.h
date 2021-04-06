#pragma once

struct point2D
{
	float		x, y;

	point2D() = default;

	point2D(float a, float b) { x = a; y = b; }

	float& operator [](int i)
	{
		return ((&x)[i]);
	}

	const float& operator [](int i) const
	{
		return ((&x)[i]);
	}
};

inline point2D operator *(const point2D& v, float s)
{
	return (point2D(v.x * s, v.y * s));
}

inline point2D operator *(float s, const point2D& v)
{
	return (point2D(v.x * s, v.y * s));
}

inline point2D operator +(const point2D& a, const point2D& b)
{
	return (point2D(a.x + b.x, a.y + b.y));
}