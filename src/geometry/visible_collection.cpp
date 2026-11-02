#include "geometry/visible_collection.hpp"

#include "aabb.hpp"

std::optional<Hit> VisibleCollection::hitCheck(const Ray& r, double t_min, double t_max) const {
	std::optional<Hit> rec = std::nullopt;
	double closest = t_max;

	for (const auto& object : objects_) {
		auto temp_rec = object->hitCheck(r, t_min, closest);
		if (temp_rec) {
			closest = temp_rec->t;
			rec = std::move(temp_rec);
		}
	}

	return rec;
}

std::optional<Aabb> VisibleCollection::boundingBox(double time0, double time1) const {
	if (objects_.empty()) return std::nullopt;
	Aabb output;
	bool first = true;

	for (const auto& object : objects_) {
		auto obj_box = object->boundingBox(time0, time1);
		if (!obj_box) return std::nullopt;
		output = first ? obj_box.value() : surroundingBox(output, obj_box.value());
		first = false;
	}
	return output;
}
