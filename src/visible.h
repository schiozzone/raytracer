#pragma once

#include <optional>

#include "ray.h"
#include "aabb.h"

class material;

struct hit {
	vec3 point;
	vec3 normal;
	std::shared_ptr<material> mat_ptr;
	double t;
	bool front_face;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class visible {
public:
	virtual std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const = 0;
	virtual std::optional<aabb> bounding_box(double time0, double time1) const = 0;
};