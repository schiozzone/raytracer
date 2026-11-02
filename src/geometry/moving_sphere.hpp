#pragma once

#include "geometry/visible.hpp"

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