#pragma once

#include <utility>

#include "geometry/visible.hpp"

#include "materials/material.hpp"
class Texture;

class ConstantMedium : public Visible {
public:
	ConstantMedium(std::shared_ptr<Visible> boundary, double density, const std::shared_ptr<Texture>& albedo)
	    : convex_(std::move(boundary)), neg_inv_density_(-1 / density), phase_function_(std::make_shared<Isotropic>(albedo)) {}
	ConstantMedium(std::shared_ptr<Visible> boundary, double density, Vec3 color)
	    : convex_(std::move(boundary)), neg_inv_density_(-1 / density), phase_function_(std::make_shared<Isotropic>(color)) {}

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override;
	std::optional<Aabb> boundingBox(double time0, double time1) const override {
		return convex_->boundingBox(time0, time1);
	}

private:
	std::shared_ptr<Visible> convex_;
	double neg_inv_density_;
	std::shared_ptr<Material> phase_function_;
};
