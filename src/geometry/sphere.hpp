#pragma once

#include <cassert>
#include <utility>

#include "geometry/visible.hpp"
#include "math/common.hpp"

class Sphere : public Visible {
public:
	Sphere() : r_(0) {};
	Sphere(Vec3 center, double radius, std::shared_ptr<Material> material)
	    : c_(center), r_(radius), m_(std::move(material)) {};

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override;
	std::optional<Aabb> boundingBox(double time0, double time1) const override;

	static std::pair<double, double> getSphereUv(const Vec3& p) {
		assert(std::fabs(p.length_squared() - 1.0) < 1e-8 && "p is not a unit vector");
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + pi;
		return {
			phi / (2 * pi),
			theta / pi
		};
	}

private:
	Vec3 c_;
	double r_;
	std::shared_ptr<Material> m_;
};
