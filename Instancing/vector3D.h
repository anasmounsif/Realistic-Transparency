#pragma once

struct vector3D
{
	float		x, y, z;

	vector3D() = default;

	vector3D(float a, float b, float c) { x = a; y = b; z = c; }

	float& operator [](int i)
	{
		return ((&x)[i]);
	}

	const float& operator [](int i) const
	{
		return ((&x)[i]);
	}

	vector3D& operator *=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return (*this);
	}

	vector3D& operator /=(float s)
	{
		s = 1.0F / s;
		x *= s;
		y *= s;
		z *= s;
		return (*this);
	}

	vector3D& operator +=(const vector3D& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return (*this);
	}

	vector3D& operator -=(const vector3D& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return (*this);
	}
};

inline vector3D operator *(const vector3D& v, float s)
{
	return (vector3D(v.x * s, v.y * s, v.z * s));
}

inline vector3D operator *(float s, const vector3D& v)
{
	return (vector3D(v.x * s, v.y * s, v.z * s));
}

inline vector3D operator /(const vector3D& v, float s)
{
	s = 1.0f / s;
	return (vector3D(v.x * s, v.y * s, v.z * s));
}

inline vector3D operator -(const vector3D& v)
{
	return (vector3D(-v.x, -v.y, -v.z));
}

inline float magnitude(const vector3D& v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline vector3D normalize(const vector3D& v)
{
	return (v / magnitude(v));
}

inline vector3D operator +(const vector3D& a, const vector3D& b)
{
	return (vector3D(a.x + b.x, a.y + b.y, a.z + b.z));
}

inline vector3D operator -(const vector3D& a, const vector3D& b)
{
	return (vector3D(a.x - b.x, a.y - b.y, a.z - b.z));
}

inline float dot(const vector3D& a, const vector3D& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline vector3D cross(const vector3D& a, const vector3D& b)
{
	return (vector3D(a.y * b.z - a.z * b.y,
									 a.z * b.x - a.x * b.z,
									 a.x * b.y - a.y * b.x));
}

inline vector3D project(const vector3D& a, const vector3D& b)
{
	return (b * (dot(a, b) / dot(b, b)));
}

inline vector3D reject(const vector3D& a, const vector3D& b)
{
	return (a - b * (dot(a, b) / dot(b, b)));
}

inline vector3D reflect(const vector3D &i, const vector3D &n)
{
	//return i - 2.0f * dot(i, n) * n;
	return  2.0f * dot(i, n) * n - i; // i vettore uscente dalla normale
}


