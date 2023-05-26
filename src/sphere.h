#pragma once

#include "visible.h"

class sphere : public visible {
public:
	sphere() : r(0) {};
	sphere(vec3 center, double radius, std::shared_ptr<material> material)
		: c(center), r(radius), m(material) {};

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override;

	static std::pair<double, double> get_sphere_uv(const vec3& p) {
		assert(std::fabs(p.length_squared() - 1.0) < std::numeric_limits<T>::min());
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + pi;
		return {
			phi / (2 * pi),
			theta / pi
		};
	}

private:
	vec3 c;
	double r;
	std::shared_ptr<material> m;
};

std::optional<hit> sphere::hit_check(const ray& r, double t_min, double t_max) const {
	vec3 o_c = r.origin() - this->c;
	double a = r.direction().length_squared(); // <v, v> = |v|^2
	double hb = dot(r.direction(), o_c);
	double c = o_c.length_squared() - this->r * this->r;
	double d = hb * hb - a * c;
	if (d < 0) { // at^2 + bt + c = 0 has no solutions
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
	vec3 outward_normal = (rec.point - this->c) / this->r;
	rec.set_face_normal(r, outward_normal);
	std::tie(rec.u, rec.v) = get_sphere_uv(outward_normal);
	rec.mat_ptr = this->m;

	return rec;
}

std::optional<aabb> sphere::bounding_box(double time0, double time1) const {
	return aabb(
		this->c - vec3(this->r, this->r, this->r),
		this->c + vec3(this->r, this->r, this->r)
	);
}