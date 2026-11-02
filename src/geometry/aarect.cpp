#include "geometry/aarect.hpp"

#include "aabb.hpp"

std::optional<hit> xy_rect::hit_check(const ray& r, double t_min, double t_max) const {
	auto t = (this->k - r.origin().z()) / r.direction().z();
	if (t < t_min || t > t_max)
		return std::nullopt;
	auto x = r.origin().x() + t * r.direction().x();
	auto y = r.origin().y() + t * r.direction().y();
	if (x < this->x0 || x > this->x1 || y < this->y0 || y > this->y1)
		return std::nullopt;

	hit rec;
	rec.t = t;
	rec.point = r.at(t);
	vec3 outward_normal = vec3(0, 0, 1);
	rec.set_face_normal(r, outward_normal);
	rec.u = (x - this->x0) / (this->x1 - this->x0);
	rec.v = (y - this->y0) / (this->y1 - this->y0);
	rec.mat_ptr = this->m;

	return rec;
}

std::optional<aabb> xy_rect::bounding_box(double /*time0*/, double /*time1*/) const {
	// Adds some padding on the z dimension
	return aabb(
		vec3(x0, y0, k - 0.0001),
		vec3(x1, y1, k + 0.0001)
	);
}


std::optional<hit> xz_rect::hit_check(const ray& r, double t_min, double t_max) const {
	auto t = (this->k - r.origin().y()) / r.direction().y();
	if (t < t_min || t > t_max)
		return std::nullopt;
	auto x = r.origin().x() + t * r.direction().x();
	auto z = r.origin().z() + t * r.direction().z();
	if (x < this->x0 || x > this->x1 || z < this->z0 || z > this->z1)
		return std::nullopt;

	hit rec;
	rec.t = t;
	rec.point = r.at(t);
	vec3 outward_normal = vec3(0, 1, 0);
	rec.set_face_normal(r, outward_normal);
	rec.u = (x - this->x0) / (this->x1 - this->x0);
	rec.v = (z - this->z0) / (this->z1 - this->z0);
	rec.mat_ptr = this->m;

	return rec;
}

std::optional<aabb> xz_rect::bounding_box(double /*time0*/, double /*time1*/) const {
	// Adds some padding on the y dimension
	return aabb(
		vec3(x0, k - 0.0001, z0),
		vec3(x1, k + 0.0001, z1)
	);
}


std::optional<hit> yz_rect::hit_check(const ray& r, double t_min, double t_max) const {
	auto t = (this->k - r.origin().x()) / r.direction().x();
	if (t < t_min || t > t_max)
		return std::nullopt;
	auto y = r.origin().y() + t * r.direction().y();
	auto z = r.origin().z() + t * r.direction().z();
	if (y < this->y0 || y > this->y1 || z < this->z0 || z > this->z1)
		return std::nullopt;

	hit rec;
	rec.t = t;
	rec.point = r.at(t);
	vec3 outward_normal = vec3(1, 0, 0);
	rec.set_face_normal(r, outward_normal);
	rec.u = (y - this->y0) / (this->y1 - this->y0);
	rec.v = (z - this->z0) / (this->z1 - this->z0);
	rec.mat_ptr = this->m;

	return rec;
}

std::optional<aabb> yz_rect::bounding_box(double /*time0*/, double /*time1*/) const {
	// Adds some padding on the x dimension
	return aabb(
		vec3(k - 0.0001, y0, z0),
		vec3(k + 0.0001, y1, z1)
	);
}