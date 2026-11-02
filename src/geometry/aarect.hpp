#pragma once

#include <cassert>

#include "geometry/visible.hpp"

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
