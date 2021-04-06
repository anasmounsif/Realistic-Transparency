#pragma once
#include "point3D.h"

class aabb {
public:
	aabb() = default;
	aabb(const point3D& a, const point3D& b) { pmin = a; pmax = b; }

	point3D aabb_min() const { return pmin; }
	point3D aabb_max() const { return pmax; }

	bool hit(const ray& r, float tmin, float tmax) const {
		for (int a = 0; a < 3; a++) {
			float t0 = ffmin((pmin[a] - r.origin()[a]) / r.direction()[a],
				(pmax[a] - r.origin()[a]) / r.direction()[a]);
			float t1 = ffmax((pmin[a] - r.origin()[a]) / r.direction()[a],
				(pmax[a] - r.origin()[a]) / r.direction()[a]);
			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);
			if (tmax <= tmin)
				return false;
		}
		return true;
	}

	point3D pmin;
	point3D pmax;
};

aabb surrounding_box(aabb box0, aabb box1)
{
	point3D small_point(ffmin(box0.pmin.x, box1.pmin.x),
		ffmin(box0.pmin.y, box1.pmin.y),
		ffmin(box0.pmin.z, box1.pmin.z));

	point3D big_point(ffmax(box0.pmax.x, box1.pmax.x),
		ffmax(box0.pmax.y, box1.pmax.y),
		ffmax(box0.pmax.z, box1.pmax.z));

	return aabb(small_point, big_point);
}
