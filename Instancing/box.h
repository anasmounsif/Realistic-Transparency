#pragma once
#include "object.h"
#include "math.h"
#include "vector3D.h"
#include "point3D.h"

class box : public object {
	box() {
		pmin = point3D(-1.0, -1.0, -1.0);
		pmax = point3D(1.0, 1.0, 1.0);
	}
	
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec) const;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max) const;

protected:
	point3D pmin;
	point3D pmax;
};

bool box::hit(const ray& r, float tmin, float tmax, hit_record &rec) const {
	for (int i = 0; i < 3; i++) {
		float t0 = ffmin((pmin[i] - r.origin()[i]) / r.direction()[i],
			(pmax[i] - r.origin()[i]) / r.direction()[i]);
		float t1 = ffmax((pmin[i] - r.origin()[i]) / r.direction()[i],
			(pmax[i] - r.origin()[i]) / r.direction()[i]);
		tmin = ffmax(t0, tmin);
		tmax = ffmin(t1, tmax);
		if (tmax <= tmin)
			return false;
	}
	return true;
}

bool box::hit_shadow(const ray& r, float tmin, float tmax) const {
	for (int i = 0; i < 3; i++) {
		float t0 = ffmin((pmin[i] - r.origin()[i]) / r.direction()[i],
			(pmax[i] - r.origin()[i]) / r.direction()[i]);
		float t1 = ffmax((pmin[i] - r.origin()[i]) / r.direction()[i],
			(pmax[i] - r.origin()[i]) / r.direction()[i]);
		tmin = ffmax(t0, tmin);
		tmax = ffmin(t1, tmax);
		if (tmax <= tmin)
			return false;
	}
	return true;
}
