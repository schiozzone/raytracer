#pragma once

#include "geometry/visible_collection.hpp"

#include "aabb.hpp"

class box : public visible {
public:
	box() {}
	box(const vec3& point0, const vec3& point1, std::shared_ptr<material> material);

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override {
		return sides.hit_check(r, t_min, t_max);
	}

	std::optional<aabb> bounding_box(double /*time0*/, double /*time1*/) const override {
		return aabb{ box_min, box_max };
	}

private:
	vec3 box_min;
	vec3 box_max;
	visible_collection sides;
};

