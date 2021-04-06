#pragma once
#include "vector3D.h"
#include "point3D.h"
#include "material.h"
#include "ray.h"
#include "record.h"
#include "aabb.h"

class object {
public:
	object() {}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max) const = 0;

	virtual bool bounding_box(aabb& box) const = 0;
};
