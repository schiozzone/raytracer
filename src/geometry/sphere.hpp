#pragma once

#include <cassert>

#include "geometry/visible.hpp"

class sphere : public visible {
public:
	sphere() : r(0) {};
	sphere(vec3 center, double radius, std::shared_ptr<material> material)
		: c(center), r(radius), m(material) {};

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override;

	static std::pair<double, double> get_sphere_uv(const vec3& p) {
		assert(std::fabs(p.length_squared() - 1.0) < 1e-8 && "p is not a unit vector");
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
