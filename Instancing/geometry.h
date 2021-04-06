#pragma once
#include "object.h"
#include "math.h"
#include "vector3D.h"
#include "point3D.h"
#include "material.h"

class plane : public object {
public:
	plane() {}
	
	plane(const point3D p, const vector3D &n) {
		point = p;
		normal = n;
}

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec) const;

private:
	point3D point;
	vector3D normal;
};

bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	float denom = dot(normalize(r.d), normal);
	if (denom > 0) {
		vector3D v = normalize(point - r.o);
		float temp = dot(v, normal) / denom;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = normal;
			return true;
		}
	}
	return false;
}


