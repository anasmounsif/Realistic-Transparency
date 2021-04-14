#pragma once
#include "color.h"
#include "ray.h"
#include "record.h"
#include "vector3D.h"
#include "point3D.h"
#include "texture.h"
#include "math.h"
#include <iostream>

struct hit_record;


class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
		return false;
	}

	virtual bool trasmitter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
		return false;
	}

	virtual color emitted(const ray& r_in, const hit_record& rec, float u, float v, const point3D& p) const { return color(0, 0, 0); }

	color ka, kd, ks;
	float alpha;
};


class lambertian : public material {
public:
	lambertian(texture* a) : albedo(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
		vector3D target = rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target);
		attenuation = albedo->value(rec.u ,rec.v, rec.p);
		return true;
	}

	texture* albedo;
};

class metal : public material {
public:
	metal(const color& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
		vector3D reflected = -reflect(normalize(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	color albedo;
	float fuzz;
};

float fresnel(const hit_record& rec, const ray& r, float eta) {
	// Calculation of the Reflection coefficient (kr)

	vector3D normal = rec.normal;
	float ndotd = dot(-normal, normalize(r.direction()));
	if (ndotd < 0.0f) {
		// Ray hits inside surface.
		normal = -normal;
	}

	float cos_theta_i = dot(-normal, normalize(r.direction()));
	float cos_theta_t = sqrt(1.0 - (1.0 - pow(cos_theta_i, 2)) / pow(eta, 2));
	if (isnan(abs(cos_theta_t))) {
		cos_theta_t = 0;
	}

	float r_parallel = ((eta * cos_theta_i) - cos_theta_t) / ((eta * cos_theta_i) + cos_theta_t);
	float r_perpendicular = (cos_theta_i - (eta * cos_theta_t)) / (cos_theta_i + (eta * cos_theta_t));
	float kr = 0.5 * (pow(r_parallel, 2) + pow(r_perpendicular, 2));
	return kr;
}

bool refract(const vector3D& v, const vector3D& n, float ni_over_nt, vector3D& refracted) {
	// Calculation of refracted ray.

	vector3D d = normalize(v);
	float dn = dot(d, n);
	float discriminant = 1.0f - (ni_over_nt * ni_over_nt * (1.0f - dn * dn));
	if (discriminant > 0) {
		refracted = ni_over_nt * (d - n * dn) - n * sqrt(discriminant);
		return true;
	}

	return false;
}

color filtering(const hit_record& rec, const ray& r, color in, color out) {
	// ***Color Filtering.
	// out: exterior filter color.
	// in: interior filter color.

	vector3D normal = rec.normal;
	if (dot(normalize(r.direction()), normal) > 0) {
		// Ray inside.
		return color(pow(out.r, rec.t), pow(out.g, rec.t), pow(out.b, rec.t));
	}
	// Ray outside.
	return color(pow(in.r, rec.t), pow(in.g, rec.t), pow(in.b, rec.t));
}

class dielectric : public material {
public:
	dielectric(float n_in, float n_out, color filter_in, color filter_out) : eta_in(n_in), eta_out(n_out), filterIn(filter_in), filterOut(filter_out) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
		// This method computes only the ray reflectionand its attenuation.

		// Reflected ray calculation.
		vector3D reflected = -reflect(r_in.direction(), rec.normal);

		float eta;
		if (dot(r_in.direction(), rec.normal) > 0) {
			// Ray reflect inside.
			eta = eta_out / eta_in;
		}
		else { 
			// Ray reflect outside.
			eta = eta_in / eta_out;
		}

		float kr = fresnel(rec, r_in, eta); 
		color filter = filtering(rec, r_in, filterIn, filterOut); 
		attenuation = kr * pow(eta, 2) * filter;
		scattered = ray(rec.p, reflected);

		// If the reflected ray has been calculated.
		return true;
	}

	virtual bool trasmitter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
		// This method computes only the ray transmitted its attenuation.

		vector3D refracted;
		float eta;
		vector3D outward_normal;

		if (dot(r_in.direction(), rec.normal) > 0) {
			// Ray reflect inside.
			eta = eta_out / eta_in;
			outward_normal = -rec.normal;
		}
		else {
			// Ray reflect outside.
			eta = eta_in / eta_out;
			outward_normal = rec.normal;
		}

		float kr = fresnel(rec, r_in, eta); 
		if (kr == 1.0f) {
			//If kr == 1.0f total internal reflection.
			// kr + kt = 1 -> kt = 1 <--> kr = 0. (For the law of conservation of energy)
			// The transmitted ray is not calculated.
			return false;
		}

		// kr + kt = 1 -> kt = 1 - kr. (For the law of conservation of energy)
		float kt = 1 - kr;

		color filter = filtering(rec, r_in, filterIn, filterOut); 
		attenuation = kt * pow(eta, 2) * filter;
		bool t = refract(r_in.direction(), outward_normal, 1 / eta, refracted); //compute ray tramitted.
		scattered = ray(rec.p, refracted); // create ray tramitted.

		// If the transmitted ray has been calculated.
		return true;
	}

	float eta_in;
	float eta_out;
	color filterIn;
	color filterOut;
};

class diffuse_light : public material {
public:
	diffuse_light(texture* a) : emit(a) {}

	virtual color emitted(const ray& r_in, const hit_record& rec, float u, float v, const point3D& p) const {
		if (dot(rec.normal, r_in.direction()) < 0.0)
			return emit->value(u, v, p);
		else
			return color(0, 0, 0);
	}

	texture* emit;
};

class specular : public material {
public:
	//specular(color ambient, color diffuse, color specular, float a) : ka(ambient), kd(diffuse), ks(specular), alpha(a) {};

	specular() {
		ka.r = randZeroToOne();
		ka.g = randZeroToOne();
		ka.b = randZeroToOne();

		kd.r = randZeroToOne();
		kd.g = randZeroToOne();
		kd.b = randZeroToOne();

		ks.r = randZeroToOne();
		ks.g = randZeroToOne();
		ks.b = randZeroToOne();

		alpha = 20 + randZeroToOne() * 200;
	}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
		return true;
	}

};


