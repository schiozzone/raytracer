#pragma once

#include "common.h"
#include "visible.h"

class xy_rect : public visible {
public:
	xy_rect() : x0(0), x1(0), y0(0), y1(0), k(0) {}
	xy_rect(double x0, double x1, double y0, double y1, double z, std::shared_ptr<material> material)
		: x0(x0), x1(x1), y0(y0), y1(y1), k(z), m(material) {
		assert(x0 <= x1);
		assert(y0 <= y1);
	};

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override;

private:
	std::shared_ptr<material> m;
	double x0, x1, y0, y1, k;
};

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

std::optional<aabb> xy_rect::bounding_box(double time0, double time1) const {
	// Adds some padding on the z dimension
	return aabb(
		vec3(x0, y0, k - 0.0001),
		vec3(x1, y1, k + 0.0001)
	);
}

class xz_rect : public visible {
public:
	xz_rect() : x0(0), x1(0), z0(0), z1(0), k(0) {}
	xz_rect(double x0, double x1, double z0, double z1, double y, std::shared_ptr<material> material)
		: x0(x0), x1(x1), z0(z0), z1(z1), k(y), m(material) {
		assert(x0 <= x1);
		assert(z0 <= z1);
	};

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override;

private:
	std::shared_ptr<material> m;
	double x0, x1, z0, z1, k;
};

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

std::optional<aabb> xz_rect::bounding_box(double time0, double time1) const {
	// Adds some padding on the y dimension
	return aabb(
		vec3(x0, k - 0.0001, z0),
		vec3(x1, k + 0.0001, z1)
	);
}

class yz_rect : public visible {
public:
	yz_rect() : y0(0), y1(0), z0(0), z1(0), k(0) {}
	yz_rect(double y0, double y1, double z0, double z1, double x, std::shared_ptr<material> material)
		: y0(y0), y1(y1), z0(z0), z1(z1), k(x), m(material) {
		assert(y0 <= y1);
		assert(z0 <= z1);
	};

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override;

private:
	std::shared_ptr<material> m;
	double y0, y1, z0, z1, k;
};

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

std::optional<aabb> yz_rect::bounding_box(double time0, double time1) const {
	// Adds some padding on the x dimension
	return aabb(
		vec3(k - 0.0001, y0, z0),
		vec3(k + 0.0001, y1, z1)
	);
}