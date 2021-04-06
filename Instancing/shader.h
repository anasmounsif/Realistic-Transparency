#pragma once
#include <iostream>
#include <time.h>
#include <algorithm>
#include "camera.h"
#include "light.h"
#include "material.h"
#include "object.h"
#include "color.h"
#include "point3D.h"
#include "vector3D.h"
#include "math.h"

color phong_shading(const point_light &light, const hit_record &hr, const camera &cam) {
	color ambient(0.0, 0.0, 0.0);
	color diffuse(0.0, 0.0, 0.0);
	color specular(0.0, 0.0, 0.0);

	ambient = hr.m->ka * light.ambient;

	vector3D L = normalize(light.position - hr.p);
	float LDotN = max(dot(L, hr.normal), 0.0f);
		
	if (LDotN > 0) {
		diffuse = hr.m->kd * light.diffuse * LDotN;

		vector3D R = reflect(L, hr.normal);

		vector3D V = normalize(cam.origin - hr.p);
		float VDotR = std::pow(max(dot(V, R), 0.0f), hr.m->alpha);
		
		specular = hr.m->ks * light.specular * VDotR;

		return ambient + diffuse + specular;
	}
	else
		return ambient;
}

color ambient_shading(const point_light &light, const hit_record &hr) {
	color ambient(0.0, 0.0, 0.0);
	color diffuse(0.0, 0.0, 0.0);
	color specular(0.0, 0.0, 0.0);

	ambient = hr.m->ka * light.ambient;

	return ambient;
}
