#pragma once

#include <memory>
#include <vector>

#include "geometry/visible.hpp"

class VisibleCollection : public Visible {
public:
	VisibleCollection() = default;
	VisibleCollection(const std::shared_ptr<Visible>& object) { add(object); }

	void clear() { objects_.clear(); }
	void add(const std::shared_ptr<Visible>& object) { objects_.push_back(object); }

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override;
	std::optional<Aabb> boundingBox(double time0, double time1) const override;

private:
	friend class BvhNode;
	std::vector<std::shared_ptr<Visible>> objects_;
};
