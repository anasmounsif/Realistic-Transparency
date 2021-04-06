#pragma once
#include "vector3D.h"
#include "point3D.h"

class ray
{
public:
	point3D o;
	vector3D d;

	ray() {}
	ray(const ray& ray) : o(ray.o), d(ray.d) {};
	ray(const point3D& _origin, const vector3D& _direction) { o = _origin; d = _direction; }
	point3D origin() const { return o; }
	vector3D direction() const { return d; }
	point3D point_at_parameter(float t) const { return o + t * d; }

};
