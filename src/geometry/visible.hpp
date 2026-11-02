#pragma once

#include <optional>

#include "math/ray.hpp"
#include "aabb.hpp"

class material;

struct hit {
	vec3 point;
	vec3 normal;
	std::shared_ptr<material> mat_ptr;
	double t;
	double u;
	double v;
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

class translate : public visible {
public:
	translate(std::shared_ptr<visible> primitive, const vec3& displacement)
		: p(primitive), offset(displacement) {}

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override;

private:
	std::shared_ptr<visible> p;
	vec3 offset;
};

class rotate_y : public visible {
public:
	rotate_y(std::shared_ptr<visible> primitive, double angle);

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double /*time0*/, double /*time1*/) const override {
		return bbox;
	}

private:
	std::shared_ptr<visible> p;
	double sin_theta;
	double cos_theta;
	std::optional<aabb> bbox;
};
