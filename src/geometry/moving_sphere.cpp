#include "geometry/moving_sphere.hpp"

#include "geometry/sphere.hpp"
#include "aabb.hpp"

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
	std::tie(rec.u, rec.v) = sphere::get_sphere_uv(outward_normal);
	rec.mat_ptr = this->m;

	return rec;
}

std::optional<aabb> moving_sphere::bounding_box(double /*time0*/, double /*time1*/) const {
	auto radius = fabs(this->r);
	return surrounding_box(
		aabb(
			this->c0 - vec3(radius, radius, radius),
			this->c0 + vec3(radius, radius, radius)
		),
		aabb(
			this->c1 - vec3(radius, radius, radius),
			this->c1 + vec3(radius, radius, radius)
		)
	);
}