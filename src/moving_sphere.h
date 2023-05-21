#pragma once

#include "common.h"
#include "visible.h"

class moving_sphere : public visible {
public:
	moving_sphere() : t0(0), t1(0), r(0) {}
	moving_sphere(vec3 center0, vec3 center1, double time0, double time1, double radius, std::shared_ptr<material> material)
		: c0(center0), c1(center1), t0(time0), t1(time1), r(radius), m(material) {}

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override;

	vec3 center(double time) const; 

private:
	vec3 c0, c1;
	double t0, t1, r;
	std::shared_ptr<material> m;
};

vec3 moving_sphere::center(double time) const {
	return c0 + (c1 - c0) * ((time - t0) / (t1 - t0));
}

std::optional<hit> moving_sphere::hit_check(const ray& r, double t_min, double t_max) const {
	vec3 o_c = r.origin() - center(r.time()); // Changed from sphere code
	double a = r.direction().length_squared();
	double hb = dot(r.direction(), o_c);
	double c = o_c.length_squared() - this->r * this->r;
	double d = hb * hb - a * c;
	if (d < 0) {
		return std::nullopt;
	}

	double d_sqrt = std::sqrt(d);
	double root = (-hb - d_sqrt) / a;
	if (root < t_min || root > t_max) {
		root = (-hb + d_sqrt) / a;
		if (root < t_min || root > t_max) return std::nullopt;
	}

	hit rec;
	rec.t = root;
	rec.point = r.at(rec.t);
	vec3 outward_normal = (rec.point - center(r.time())) / this->r; // Changed from sphere code
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = this->m;

	return rec;
}

std::optional<aabb> moving_sphere::bounding_box(double time0, double time1) const {
	return surrounding_box(
		aabb(
			this->c0 - vec3(this->r, this->r, this->r),
			this->c0 + vec3(this->r, this->r, this->r)
		),
		aabb(
			this->c1 - vec3(this->r, this->r, this->r),
			this->c1 + vec3(this->r, this->r, this->r)
		)
	);
}