#pragma once
#include <math.h> 
#include "object.h"
#include "math.h"
#include "vector3D.h"
#include "point3D.h"

class disk : public object {
public:
	disk() {}
	disk(point3D c, vector3D n, float r) : center(c), normal(n), radius(r) {};

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max) const;

	virtual bool bounding_box(aabb& box) const;

protected:
	point3D center;
	vector3D normal;
	float radius;
};

bool disk::bounding_box(aabb& box) const {
	
	return true;
}

bool disk::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	if (dot(r.direction(), normal) != 0) {
		vector3D dist = center - r.origin();
		float temp = dot(dist, normal) / dot(r.direction(), normal);
		point3D p = r.point_at_parameter(temp);
		vector3D distP = p - center;
		if (temp < t_max && temp > t_min&& magnitude(distP) <= radius) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = normal;

			return true;
		}
	}
	return false;

}

bool disk::hit_shadow(const ray& r, float t_min, float t_max) const {
	return false;
}
