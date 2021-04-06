#pragma once
#include "ray.h"
#include "vector3D.h"
#include "point3D.h"
#include "math.h"


class camera {
public:
	camera(point3D pos, point3D lookat, vector3D up, float vfov, float aspect) {
		float theta = vfov * float(M_PI) / 180.0f;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = pos;
		w = normalize(pos - lookat);
		u = normalize(cross(up, w));
		v = cross(w, u);
		left = -half_width*u - half_height*v - w;
		horizontal = 2.0f * half_width*u;
		vertical = 2.0f * half_height*v;
	}

	ray get_ray(float u, float v) {
		return ray(origin, left + u*horizontal + v*vertical);
	}

	point3D origin;
	vector3D left;
	vector3D u, v, w;

	vector3D horizontal, vertical;
};

