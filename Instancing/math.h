#pragma once
#include <stdlib.h>
#include "color.h"
#include "vector3D.h"

extern int fail;

const float Deg2Rad = 3.1415926f / 180.0f;

inline float lerp(float a, float b, float t) {
	return (1.0f - t) * a + t * b;
}

inline vector3D lerp(vector3D &a, vector3D &b, float t) {
	return (1.0f - t) * a + t * b;
}

inline color lerp(color &a, color &b, float t) {
	return (1.0f - t) * a + t * b;
}

float randZeroToOne(){
	return float(rand() / (RAND_MAX + 1.));
}

double randMToN(double M, double N){
	return M + (rand() / (RAND_MAX / (N - M)));
}

double randMinusOneToOne() {
	return randMToN(-1.0, 1.0);
}

float max(const float a, const float b)
{
	return (a < b) ? b : a;
}


vector3D random_in_unit_sphere() {
	vector3D p;
	
	do {
		p = 2.0f*vector3D(randZeroToOne(), randZeroToOne(), randZeroToOne()) - vector3D(1, 1, 1);
	} while (magnitude(p) >= 1.0f);
	return p;
}

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }



