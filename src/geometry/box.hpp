#pragma once

#include "geometry/visible_collection.hpp"

#include "aabb.hpp"

class Box : public Visible {
public:
	Box() = default;
	Box(const Vec3& point0, const Vec3& point1, std::shared_ptr<Material> material);

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override {
		return sides_.hitCheck(r, t_min, t_max);
	}

	std::optional<Aabb> boundingBox(double /*time0*/, double /*time1*/) const override {
		return Aabb{ box_min_, box_max_ };
	}

private:
	Vec3 box_min_;
	Vec3 box_max_;
	VisibleCollection sides_;
};
