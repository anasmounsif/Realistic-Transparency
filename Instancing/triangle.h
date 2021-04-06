#pragma once
#include "object.h"
#include "math.h"
#include "vector3D.h"
#include "point3D.h"
#include "point2D.h"

class triangle : public object {
public:

	triangle() {
		v0.x = 0.0; v0.y = 1.0; v0.z = 0.0;
		v1.x = -1.0; v1.y = 0.0; v1.z = 0.0;
		v2.x = 1.0; v1.y = 0.0; v1.z = 0.0;
	}

	triangle(point3D _vertices[3]) {
		v0 = _vertices[0];
		v1 = _vertices[1];
		v2 = _vertices[2];
		norm = normalize(cross(v1 - v0, v2 - v0));
	};

	triangle(point3D _v0, point3D _v1, point3D _v2) {
		v0 = _v0;
		v1 = _v1;
		v2 = _v2;
		norm = normalize(cross(v1 - v0, v2 - v0));
	};

	triangle(point3D _v0, point2D _uv0, point3D _v1, point2D _uv1, point3D _v2, point2D _uv2) {
		v0 = _v0; v1 = _v1; v2 = _v2;
		uv0 = _uv0; uv1 = _uv1; uv2 = _uv2;
		norm = normalize(cross(v1 - v0, v2 - v0));
	};

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec) const;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max) const;

	virtual bool bounding_box(aabb& box) const;

protected:
	point3D v0, v1, v2;
	point2D uv0, uv1, uv2;
	vector3D norm;
};

bool triangle::bounding_box(aabb& box) const
{
	box.pmax = point3D(FLT_MIN, FLT_MIN, FLT_MIN);
	box.pmin = point3D(FLT_MAX, FLT_MAX, FLT_MAX);

	box.pmin.x = ffmin(v0.x, box.pmin.x);
	box.pmin.y = ffmin(v0.y, box.pmin.y);
	box.pmin.z = ffmin(v0.z, box.pmin.z);
	box.pmin.x = ffmin(v1.x, box.pmin.x);
	box.pmin.y = ffmin(v1.y, box.pmin.y);
	box.pmin.z = ffmin(v1.z, box.pmin.z);
	box.pmin.x = ffmin(v2.x, box.pmin.x);
	box.pmin.y = ffmin(v2.y, box.pmin.y);
	box.pmin.z = ffmin(v2.z, box.pmin.z);

	box.pmax.x = ffmax(v0.x, box.pmax.x);
	box.pmax.y = ffmax(v0.y, box.pmax.y);
	box.pmax.z = ffmax(v0.z, box.pmax.z);
	box.pmax.x = ffmax(v1.x, box.pmax.x);
	box.pmax.y = ffmax(v1.y, box.pmax.y);
	box.pmax.z = ffmax(v1.z, box.pmax.z);
	box.pmax.x = ffmax(v2.x, box.pmax.x);
	box.pmax.y = ffmax(v2.y, box.pmax.y);
	box.pmax.z = ffmax(v2.z, box.pmax.z);

	return true;
}

bool triangle::hit(const ray& r, float tmin, float tmax, hit_record &rec) const
{
	const float EPSILON = 0.0000001f;
	vector3D edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = v1 - v0;
	edge2 = v2 - v0;
	h = cross(r.d, edge2);
	a = dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return false;
	f = 1 / a;
	s = r.o - v0;
	u = f * (dot(s, h));
	if (u < 0.0f || u > 1.0f)
		return false;
	q = cross(s, edge1);
	v = f * dot(r.d, q);
	if (v < 0.0f || u + v > 1.0f)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * dot(edge2, q);
	if (t > tmin && t < tmax) {
		if (t > EPSILON) // ray intersection
		{
			point3D intersPoint = r.o + (normalize(r.d) * (t * magnitude(r.d)));
			rec.normal = norm;
			rec.t = dot((v0 - r.o), norm) / dot(r.direction(), norm);
			rec.p = intersPoint;
			rec.u = u * uv1.x + v * uv2.x + (1 - u - v) * uv0.x;
			rec.v = u * uv1.y + v * uv2.y + (1 - u - v) * uv0.y;
			return true;
		}
	}
	// This means that there is a line intersection but not a ray intersection.
	return false;
}

bool triangle::hit_shadow(const ray& r, float tmin, float tmax) const
{
	const float EPSILON = 0.0000001f;
	vector3D edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = v1 - v0;
	edge2 = v2 - v0;
	h = cross(r.d, edge2);
	a = dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return false;
	f = 1.0f / a;
	s = r.o - v0;
	u = f * (dot(s, h));
	if (u < 0.0f || u > 1.0f)
		return false;
	q = cross(s, edge1);
	v = f * dot(r.d, q);
	if (v < 0.0f || u + v > 1.0f)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * dot(edge2, q);
	if (t > tmin && t < tmax) {
		if (t > EPSILON) // ray intersection
		{
			//point3D intersPoint = r.o + (normalize(r.d) * (t * magnitude(r.d)));
			//rec.normal = norm;
			//rec.t = dot((vertices[0] - r.o), norm) / dot(r.direction(), norm);
			//rec.p = intersPoint;
			return true;
		}
	}
	// This means that there is a line intersection but not a ray intersection.
	return false;
}

