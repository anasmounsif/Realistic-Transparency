#pragma once
#include <vector>
#include "object.h"

using namespace std;

class object_list {
public:
	object_list() {}
	object_list(object **l, int n) { list = l; list_size = n; }
	
	bool trace_ray(const ray& r, float tmin, float tmax, hit_record& rec, vector<object *>& v) const;
	bool trace_shadow_ray(const ray& r, vector<object *>& v) const;
	
	object **list;
	int list_size;
};

bool object_list::trace_ray(const ray& r, float t_min, float t_max, hit_record& rec, vector<object *>& v) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	float closest_so_far = t_max;

	for (int i = 0; i < v.size(); i++) {
		if (v[i]->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	
	return hit_anything;
}

bool object_list::trace_shadow_ray(const ray& r, vector<object *>& v) const
{
	hit_record temp_rec;
	float closest_so_far = FLT_MAX;
	float t_min = 0.0;

	for (int i = 0; i < v.size(); i++) {
		if (v[i]->hit(r, t_min, closest_so_far, temp_rec)) {
			return true;
			//closest_so_far = temp_rec.t;
			//rec = temp_rec;
		}
	}

	return false;
}
