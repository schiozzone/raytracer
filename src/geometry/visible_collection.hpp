#pragma once

#include <memory>
#include <vector>

#include "geometry/visible.hpp"

class visible_collection : public visible {
public:
	visible_collection() {};
	visible_collection(std::shared_ptr<visible> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<visible> object) { objects.push_back(object); }

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override;

private:
	friend class bvh_node;
	std::vector<std::shared_ptr<visible>> objects;
};
