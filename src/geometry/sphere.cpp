#include "geometry/sphere.hpp"

#include "aabb.hpp"

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

std::optional<aabb> sphere::bounding_box(double /*time0*/, double /*time1*/) const {
	auto radius = fabs(this->r);
	return aabb(
		this->c - vec3(radius, radius, radius),
		this->c + vec3(radius, radius, radius)
	);
}