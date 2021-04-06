#pragma once
#include "object.h"
#include "math.h"
#include "triangle.h"
#include "vector3D.h"
#include "point3D.h"
#include "aabb.h"

class rectangle : public object {

// Counterclockwise rotation -> normal outgoing from screen.
// v3-----v2
// |       |
// v0-----v1

public:
	rectangle() {}
	
	rectangle(point3D v0, point3D v1, point3D v2, point3D v3) : tri_a(v0, point2D(0.0, 0.0), v1, point2D(1.0, 0.0), v2, point2D(1.0, 1.0)),
		tri_b(v0, point2D(0.0, 0.0), v2, point2D(1.0, 1.0), v3, point2D(0.0, 1.0)) {};
	
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max) const;
	
	virtual bool bounding_box(aabb& box) const {
		aabb box_a, box_b;
				
		tri_a.bounding_box(box_a);
		tri_b.bounding_box(box_b);

		box = surrounding_box(box_a, box_b);

		return true;
	}
	
	triangle tri_a, tri_b;
};

bool rectangle::hit(const ray& r, float tmin, float tmax, hit_record &rec) const {
	if (tri_a.hit(r, tmin, tmax, rec))
		return true;
	else
		return tri_b.hit(r, tmin, tmax, rec);
}


bool rectangle::hit_shadow(const ray& r, float tmin, float tmax) const {
	if (tri_a.hit_shadow(r, tmin, tmax))
		return true;
	else
		return tri_b.hit_shadow(r, tmin, tmax);
}



