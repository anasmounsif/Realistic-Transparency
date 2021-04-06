#pragma once
#include <iostream>
#include <iostream>
#include <time.h>
#include <vector>
#include "aabb.h"
#include "record.h"
#include "instance.h"
#include "object.h"

using namespace std;

class bvh_node : public instance {
public:
	bvh_node() = default;
	bvh_node::bvh_node(vector<instance *> &objs_vector, int i, int j);
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool bounding_box(aabb& box) const;
	instance *left;
	instance *right;
	aabb box;
};

bool bvh_node::bounding_box(aabb& b) const {
	b = box;
	return true;
}

bool bvh_node::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	if (box.hit(r, t_min, t_max)) {
		hit_record left_rec, right_rec;
		bool hit_left = left->hit(r, t_min, t_max, left_rec);
		bool hit_right = right->hit(r, t_min, t_max, right_rec);
		if (hit_left && hit_right) {
			if (left_rec.t < right_rec.t)
				rec = left_rec;
			else
				rec = right_rec;
			return true;
		}
		else if (hit_left) {
			rec = left_rec;
			return true;
		}
		else if (hit_right) {
			rec = right_rec;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

static bool box_x_compare(const instance *a, const instance *b) {
	aabb box_left, box_right;

	if (!a->bounding_box(box_left) || !b->bounding_box(box_right))
		cout << "no bounding box in bvh_node constructor\n";
	return (box_left.pmin.x - box_right.pmin.x < 0.0);
}

static bool box_y_compare(const instance *a, const instance *b)
{
	aabb box_left, box_right;

	if (!a->bounding_box(box_left) || !b->bounding_box(box_right))
		cout << "no bounding box in bvh_node constructor\n";
	return (box_left.pmin.y - box_right.pmin.y < 0.0);
}

static bool box_z_compare(const instance *a, const instance *b)
{
	aabb box_left, box_right;

	if (!a->bounding_box(box_left) || !b->bounding_box(box_right))
		cout << "no bounding box in bvh_node constructor\n";
	return (box_left.pmin.z - box_right.pmin.z < 0.0);
}

bvh_node::bvh_node(vector<instance *> &objs_vector, int i, int j) {
	int n = j - i + 1;
	int axis = int(3 * randZeroToOne());

	if (axis == 0)
		sort(objs_vector.begin() + i, objs_vector.begin() + j, box_x_compare);
	else if (axis == 1)
		sort(objs_vector.begin() + i, objs_vector.begin() + j, box_y_compare);
	else
		sort(objs_vector.begin() + i, objs_vector.begin() + j, box_z_compare);

	if (n == 1) {
		left = right = objs_vector[i];
	}
	else if (n == 2) {
		left = objs_vector[i];
		right = objs_vector[i + 1];
	}
	else {
		left = new bvh_node(objs_vector, i, i + (n / 2));
		right = new bvh_node(objs_vector, i + (n / 2), j);
	}

	aabb box_left, box_right;

	if (!left->bounding_box(box_left) || !right->bounding_box(box_right))
		cout << "no bounding box in bvh_node constructor\n";
	box = surrounding_box(box_left, box_right);
}


