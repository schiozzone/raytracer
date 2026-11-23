#pragma once

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
