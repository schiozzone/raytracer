#pragma once

#include "geometry/visible.hpp"

#include "materials/material.hpp"
class texture;

class constant_medium : public visible {
public:
	constant_medium(std::shared_ptr<visible> boundary, double density, std::shared_ptr<texture> albedo)
		: convex(boundary), neg_inv_density(-1 / density), phase_function(std::make_shared<isotropic>(albedo)) {}
	constant_medium(std::shared_ptr<visible> boundary, double density, vec3 color)
		: convex(boundary), neg_inv_density(-1 / density), phase_function(std::make_shared<isotropic>(color)) {}

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override {
		return convex->bounding_box(time0, time1);
	}

private:
	std::shared_ptr<visible> convex;
	double neg_inv_density;
	std::shared_ptr<material> phase_function;
};
