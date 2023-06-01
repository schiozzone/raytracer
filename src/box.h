#pragma once

#include "common.h"

#include "aarect.h"
#include "visible_collection.h"

class box : public visible {
public:
	box() {}
	box(const vec3& point0, const vec3& point1, std::shared_ptr<material> material);

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override {
		return sides.hit_check(r, t_min, t_max);
	}

	std::optional<aabb> bounding_box(double time0, double time1) const override {
		return aabb{ box_min, box_max };
	}

private:
	vec3 box_min;
	vec3 box_max;
	visible_collection sides;
};

box::box(const vec3& p0, const vec3& p1, std::shared_ptr<material> mat) {
	box_min = p0;
	box_max = p1;
	sides.add(std::make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat));
	sides.add(std::make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));
	sides.add(std::make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat));
	sides.add(std::make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
	sides.add(std::make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat));
	sides.add(std::make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));
}
