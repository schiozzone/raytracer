#pragma once

#include <algorithm>

#include "math/common.hpp"
#include "geometry/visible_collection.hpp"

class BvhNode : public Visible {
public:
	BvhNode() = default;
	BvhNode(const VisibleCollection& visibles, double time0, double time1)
	    : BvhNode(visibles.objects_, 0, visibles.objects_.size(), time0, time1) {}

	BvhNode(const std::vector<std::shared_ptr<Visible>>& objects, size_t start, size_t end, double time0, double time1);

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override;
	std::optional<Aabb> boundingBox(double /*time0*/, double /*time1*/) const override { return box_; }

private:
	std::shared_ptr<Visible> left_;
	std::shared_ptr<Visible> right_;
	Aabb box_;
};

std::optional<Hit> BvhNode::hitCheck(const Ray& r, double t_min, double t_max) const {
	if (!box_.hitCheck(r, t_min, t_max)) return std::nullopt;
	auto hit_left = left_->hitCheck(r, t_min, t_max);
	auto hit_right = right_->hitCheck(r, t_min, hit_left ? hit_left->t : t_max);
	return hit_right ? hit_right : hit_left;
}

inline bool boxCompare(const std::shared_ptr<Visible> a, const std::shared_ptr<Visible> b, int axis) {
	auto box_a = a->boundingBox(0, 0);
	auto box_b = b->boundingBox(0, 0);
	if (!box_a || !box_b) std::cerr << "No bounding box in bvh_node constructor.\n";
	return box_a.value().min()[axis] < box_b.value().min()[axis];
}

inline bool boxXCompare(const std::shared_ptr<Visible> a, const std::shared_ptr<Visible> b) {
	return boxCompare(a, b, 0);
}

inline bool boxYCompare(const std::shared_ptr<Visible> a, const std::shared_ptr<Visible> b) {
	return boxCompare(a, b, 1);
}

inline bool boxZCompare(const std::shared_ptr<Visible> a, const std::shared_ptr<Visible> b) {
	return boxCompare(a, b, 2);
}

BvhNode::BvhNode(const std::vector<std::shared_ptr<Visible>>& src, size_t start, size_t end, double time0, double time1) {
	auto objects = src;

	int axis = randomInt(0, 2);
	auto comparator =
	    (axis == 0) ? boxXCompare : (axis == 1) ? boxYCompare
	                                              : boxZCompare;

	size_t span = end - start;

	if (span <= 3) {
		left_ = objects[start]; // Order doesn't really matter here
		if (span == 1)
			right_ = left_;
		else if (span == 2)
			right_ = objects[start + 1];
		else
			right_ = std::make_shared<BvhNode>(objects, start + 1, end, time0, time1);
	}
	else {
		std::sort(objects.begin() + start, objects.begin() + end, comparator);
		auto mid = start + span / 2;
		left_ = make_shared<BvhNode>(objects, start, mid, time0, time1);
		right_ = make_shared<BvhNode>(objects, mid, end, time0, time1);
	}

	auto box_left = left_->boundingBox(time0, time1);
	auto box_right = right_->boundingBox(time0, time1);
	if (!box_left || !box_right) std::cerr << "No bounding box in bvh_node constructor.\n";

	box_ = surroundingBox(box_left.value(), box_right.value());
}
