#pragma once

#include "visible.h"

#include <memory>
#include <vector>

class visible_collection : public visible {
public:
	visible_collection() {};
	visible_collection(std::shared_ptr<visible> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<visible> object) { objects.push_back(object); }

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;

private:
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