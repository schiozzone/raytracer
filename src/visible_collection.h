#pragma once

#include <memory>
#include <vector>

#include "visible.h"
#include "aabb.h"

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

std::optional<hit> visible_collection::hit_check(const ray& r, double t_min, double t_max) const {
	std::optional<hit> rec = std::nullopt;
	double closest = t_max;

	for (const auto& object : objects) {
		auto temp_rec = object->hit_check(r, t_min, closest);
		if (temp_rec) {
			closest = temp_rec->t;
			rec = std::move(temp_rec);
		}
	}

	return rec;
}


std::optional<aabb> visible_collection::bounding_box(double time0, double time1) const {
	if (objects.empty()) return std::nullopt;
	aabb output;
	bool first = true;

	for (const auto& object : objects) {
		auto obj_box = object->bounding_box(time0, time1);
		if (!obj_box) return std::nullopt;
		output = first ? obj_box.value() : surrounding_box(output, obj_box.value());
		first = false;
	}
	return output;
}