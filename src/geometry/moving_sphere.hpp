#pragma once

#include <utility>

#include "geometry/visible.hpp"

class MovingSphere : public Visible {
public:
	MovingSphere() : t0_(0), t1_(0), r_(0) {}
	MovingSphere(Vec3 center0, Vec3 center1, double time0, double time1, double radius, std::shared_ptr<Material> material)
	    : c0_(center0), c1_(center1), t0_(time0), t1_(time1), r_(radius), m_(std::move(material)) {}

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override;
	std::optional<Aabb> boundingBox(double time0, double time1) const override;

	Vec3 center(double time) const;

private:
	Vec3 c0_, c1_;
	double t0_, t1_, r_;
	std::shared_ptr<Material> m_;
};
