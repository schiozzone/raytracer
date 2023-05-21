#pragma once

#include "common.h"

class aabb {
public:
	aabb() {}
	aabb(const vec3& minimum, const vec3& maximum) : minimum(minimum), maximum(maximum) {}

	vec3 min() const { return minimum; }
	vec3 max() const { return maximum; }

	bool hit_check(const ray& r, double t_min, double t_max) const;

	vec3 minimum, maximum;
};

inline bool aabb::hit_check(const ray& r, double t_min, double t_max) const {
	for (int a = 0; a < 3; ++a) {
		auto d = 1.0 / r.direction()[a];
		auto t0 = (minimum[a] - r.origin()[a]) * d;
		auto t1 = (maximum[a] - r.origin()[a]) * d;
		if (d < 0.0) std::swap(t0, t1);
		t_min = fmax(t0, t_min);
		t_max = fmin(t1, t_max);
		if (t_max <= t_min) return false;
	}
	return true;
}

aabb surrounding_box(aabb box0, aabb box1) {
	vec3 new_min(
		fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));
	vec3 new_max(
		fmax(box0.max().x(), box1.max().x()),
		fmax(box0.max().y(), box1.max().y()),
		fmax(box0.max().z(), box1.max().z()));
	return { new_min, new_max };
}