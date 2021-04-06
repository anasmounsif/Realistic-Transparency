#pragma once
#include "ray.h"
#include "material.h"
#include "matrix4d.h"
#include "object.h"
#include "record.h"
#include "texture.h"
#include "aabb.h"

class instance { //: public object {
public:
	instance() = default;
	instance(object* nobj_ptr, material* m);
	instance* instance::clone(void) const;

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max) const;

	virtual bool bounding_box(aabb& box) const;

	void identity();

	void translate(const vector3D& trans);
	void translate(const float dx, const float dy, const float dz);

	void scale(const vector3D& s);
	void scale(const float a, const float b, const float c);

	void rotate_x(const float r);
	void rotate_y(const float r);
	void rotate_z(const float r);

	void setMaterial(material* m) {
		mat = m;
	}

	material* getMaterial() {
		return(mat);
	}

private:
	object				*object_ptr;		// object to be transformed
	matrix4D			inverse_matrix;		// inverse transformation matrix
	matrix4D			current_matrix; 	// current transformation matrix
	material			*mat;				// material					
};

instance* instance::clone(void) const {
	return (new instance(*this));
}

instance::instance(object* nobj_ptr, material* m) {
	object_ptr = nobj_ptr;
	mat = m;
}

bool instance::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	ray inv_ray;

	inv_ray.o = inverse_matrix * r.o;
	inv_ray.d = inverse_matrix * r.d;

	if (object_ptr->hit(inv_ray, t_min, t_max, rec)) {
		rec.p = current_matrix * inv_ray.point_at_parameter(rec.t);
		rec.normal = normalize(transponse(inverse_matrix) * rec.normal);
		rec.m = mat;

		return (true);
	}

	return (false);
}


bool instance::hit_shadow(const ray& r, float t_min, float t_max) const {
	ray inv_ray;

	inv_ray.o = inverse_matrix * r.o;
	inv_ray.d = inverse_matrix * r.d;

	if (object_ptr->hit_shadow(inv_ray, t_min, t_max))
		return (true);

	return (false);
}

bool instance::bounding_box(aabb& box) const {
	object_ptr->bounding_box(box);
	box.pmin = current_matrix * box.pmin;
	box.pmax = current_matrix * box.pmax;
	return true;
}


void instance::identity() {
	set_identity(current_matrix);
	set_identity(inverse_matrix);
}

void instance::translate(const vector3D& trans) {
	translate(trans.x, trans.y, trans.z);
}

void instance::translate(const float dx, const float dy, const float dz) {

	matrix4D inv_translation_matrix;				// temporary inverse translation matrix	

	inv_translation_matrix.m[0][3] = -dx;
	inv_translation_matrix.m[1][3] = -dy;
	inv_translation_matrix.m[2][3] = -dz;

	inverse_matrix = inverse_matrix * inv_translation_matrix;

	matrix4D translation_matrix;					// temporary translation matrix4D	

	translation_matrix.m[0][3] = dx;
	translation_matrix.m[1][3] = dy;
	translation_matrix.m[2][3] = dz;

	current_matrix = translation_matrix * current_matrix;
}

void instance::scale(const vector3D& s) {
	scale(s.x, s.y, s.z);
}

void instance::scale(const float a, const float b, const float c) {

	matrix4D inv_scaling_matrix;					// temporary inverse scaling matrix4D

	inv_scaling_matrix.m[0][0] = 1.0f / a;
	inv_scaling_matrix.m[1][1] = 1.0f / b;
	inv_scaling_matrix.m[2][2] = 1.0f / c;

	inverse_matrix = inverse_matrix * inv_scaling_matrix;

	matrix4D scaling_matrix;						// temporary scaling matrix4D

	scaling_matrix.m[0][0] = a;
	scaling_matrix.m[1][1] = b;
	scaling_matrix.m[2][2] = c;

	current_matrix = scaling_matrix * current_matrix;
}

void instance::rotate_x(const float theta) {

	float sin_theta = sin(theta * Deg2Rad);
	float cos_theta = cos(theta * Deg2Rad);

	matrix4D inv_x_rotation_matrix;					// temporary inverse rotation matrix4D about x axis

	inv_x_rotation_matrix.m[1][1] = cos_theta;
	inv_x_rotation_matrix.m[1][2] = sin_theta;
	inv_x_rotation_matrix.m[2][1] = -sin_theta;
	inv_x_rotation_matrix.m[2][2] = cos_theta;

	inverse_matrix = inverse_matrix * inv_x_rotation_matrix;

	matrix4D x_rotation_matrix;						// temporary rotation matrix4D about x axis

	x_rotation_matrix.m[1][1] = cos_theta;
	x_rotation_matrix.m[1][2] = -sin_theta;
	x_rotation_matrix.m[2][1] = sin_theta;
	x_rotation_matrix.m[2][2] = cos_theta;

	current_matrix = x_rotation_matrix * current_matrix;
	//current_matrix = current_matrix * x_rotation_matrix;
}

void instance::rotate_y(const float theta) {

	float sin_theta = sin(theta * Deg2Rad);
	float cos_theta = cos(theta * Deg2Rad);

	matrix4D inv_y_rotation_matrix;					// temporary inverse rotation matrix4D about y axis

	inv_y_rotation_matrix.m[0][0] = cos_theta;
	inv_y_rotation_matrix.m[0][2] = -sin_theta;
	inv_y_rotation_matrix.m[2][0] = sin_theta;
	inv_y_rotation_matrix.m[2][2] = cos_theta;

	inverse_matrix = inverse_matrix * inv_y_rotation_matrix;

	matrix4D y_rotation_matrix;						// temporary rotation matrix4D about y axis

	y_rotation_matrix.m[0][0] = cos_theta;
	y_rotation_matrix.m[0][2] = sin_theta;
	y_rotation_matrix.m[2][0] = -sin_theta;
	y_rotation_matrix.m[2][2] = cos_theta;

	current_matrix = current_matrix * y_rotation_matrix;
}

void instance::rotate_z(const float theta) {
	float sin_theta = sin(theta * Deg2Rad);
	float cos_theta = cos(theta * Deg2Rad);

	matrix4D inv_z_rotation_matrix;					// temporary inverse rotation matrix4D about y axis	

	inv_z_rotation_matrix.m[0][0] = cos_theta;
	inv_z_rotation_matrix.m[0][1] = sin_theta;
	inv_z_rotation_matrix.m[1][0] = -sin_theta;
	inv_z_rotation_matrix.m[1][1] = cos_theta;

	inverse_matrix = inverse_matrix * inv_z_rotation_matrix;

	matrix4D z_rotation_matrix;						// temporary rotation matrix4D about y axis

	z_rotation_matrix.m[0][0] = cos_theta;
	z_rotation_matrix.m[0][1] = -sin_theta;
	z_rotation_matrix.m[1][0] = sin_theta;
	z_rotation_matrix.m[1][1] = cos_theta;

	current_matrix = z_rotation_matrix * current_matrix;
}