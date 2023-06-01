#pragma once

#include "common.h"

#include "visible.h"
#include "material.h"
#include "texture.h"

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

std::optional<hit> constant_medium::hit_check(const ray& r, double t_min, double t_max) const {
	auto rec1 = convex->hit_check(r, -infinity, infinity);
	if (!rec1) return std::nullopt;
	auto rec2 = convex->hit_check(r, rec1->t + 0.0001, infinity);
	if (!rec2) return std::nullopt;

	if (rec1->t < t_min) rec1->t = t_min;
	if (rec2->t > t_max) rec2->t = t_max;
	if (rec1->t < 0) rec1->t = 0;
	if (rec1->t >= rec2->t) return std::nullopt;

	const auto ray_length = r.direction().length();
	const auto distance_inside_boundary = (rec2->t - rec1->t) * ray_length;
	const auto hit_distance = neg_inv_density * log(random_double());
	if (hit_distance > distance_inside_boundary) return std::nullopt;

	hit rec;
	rec.t = rec1->t + hit_distance / ray_length;
	rec.point = r.at(rec.t);
	rec.normal = vec3{ 1,0,0 };// random_in_unit_sphere();
	rec.front_face = true; // Arbitrary
	rec.mat_ptr = phase_function;
	return rec;
}
