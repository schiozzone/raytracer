#pragma once

#include "common.h"
#include "visible.h"
#include "visible_collection.h"

class bvh_node : public visible {
public:
	bvh_node() {}
	bvh_node(const visible_collection& visibles, double time0, double time1)
		: bvh_node(visibles.objects, 0, visibles.objects.size(), time0, time1) {}

	bvh_node(const std::vector<std::shared_ptr<visible>>& objects, size_t start, size_t end, double time0, double time1);

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override { return box; }

private:
	std::shared_ptr<visible> left;
	std::shared_ptr<visible> right;
	aabb box;
};

std::optional<hit> bvh_node::hit_check(const ray& r, double t_min, double t_max) const {
	if (!box.hit_check(r, t_min, t_max)) return std::nullopt;
	auto hit_left = left->hit_check(r, t_min, t_max);
	auto hit_right = right->hit_check(r, t_min, hit_left ? hit_left->t : t_max);
	return hit_right ? hit_right : hit_left;
}

inline bool box_compare(const std::shared_ptr<visible> a, const std::shared_ptr<visible> b, int axis) {
	auto box_a = a->bounding_box(0, 0);
	auto box_b = b->bounding_box(0, 0);
	if (!box_a || !box_b) std::cerr << "No bounding box in bvh_node constructor.\n";
	return box_a.value().min()[axis] < box_b.value().min()[axis];
}

bool box_x_compare(const std::shared_ptr<visible> a, const std::shared_ptr<visible> b) {
	return box_compare(a, b, 0);
}

bool box_y_compare(const std::shared_ptr<visible> a, const std::shared_ptr<visible> b) {
	return box_compare(a, b, 1);
}

bool box_z_compare(const std::shared_ptr<visible> a, const std::shared_ptr<visible> b) {
	return box_compare(a, b, 2);
}

bvh_node::bvh_node(const std::vector<std::shared_ptr<visible>>& src, size_t start, size_t end, double time0, double time1) {
	auto objects = src;

	int axis = random_int(0, 2);
	auto comparator =
		(axis == 0) ? box_x_compare :
		(axis == 1) ? box_y_compare :
		              box_z_compare;

	size_t span = end - start;

	if (span <= 3) {
		left = objects[start]; // Order doesn't really matter here
		if (span == 1)
			right = left;
		else if (span == 2)
			right = objects[start + 1];
		else
			right = std::make_shared<bvh_node>(objects, start + 1, end, time0, time1);
	}
	else {
		std::sort(objects.begin() + start, objects.begin() + end, comparator);
		auto mid = start + span / 2;
		left = make_shared<bvh_node>(objects, start, mid, time0, time1);
		right = make_shared<bvh_node>(objects, mid, end, time0, time1);
	}

	auto box_left = left->bounding_box(time0, time1);
	auto box_right = right->bounding_box(time0, time1);
	if (!box_left || !box_right) std::cerr << "No bounding box in bvh_node constructor.\n";

	box = surrounding_box(box_left.value(), box_right.value());
}
