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

std::optional<hit> translate::hit_check(const ray& r, double t_min, double t_max) const {
	ray offset_r{ r.origin() - offset, r.direction(), r.time() };
	auto rec = p->hit_check(offset_r, t_min, t_max);
	if (rec) {
		rec->point += offset;
		rec->set_face_normal(offset_r, rec->normal);
	}
	return rec;
}

std::optional<aabb> translate::bounding_box(double time0, double time1) const {
	auto output_box = p->bounding_box(time0, time1);
	if (output_box) {
		output_box.emplace(
			output_box->min() + offset,
			output_box->max() + offset
		);
	}
	return output_box;
}

class rotate_y : public visible {
public:
	rotate_y(std::shared_ptr<visible> primitive, double angle);

	std::optional<hit> hit_check(const ray& r, double t_min, double t_max) const override;
	std::optional<aabb> bounding_box(double time0, double time1) const override {
		return bbox;
	}

private:
	std::shared_ptr<visible> p;
	double sin_theta;
	double cos_theta;
	std::optional<aabb> bbox;
};

rotate_y::rotate_y(std::shared_ptr<visible> primitive, double angle) : p(primitive) {
	auto radians = degrees_to_radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	bbox = p->bounding_box(0, 1); // Fixed time interval

	vec3 min{ infinity, infinity, infinity };
	vec3 max{ -infinity, -infinity, -infinity };
	
	if (bbox)
		for (auto x : { bbox->min().x(),bbox->max().x() })
			for (auto y : { bbox->min().y(),bbox->max().y() })
				for (auto z : { bbox->min().z(),bbox->max().z() }) {
					auto newx = cos_theta * x + sin_theta * z;
					auto newz = -sin_theta * x + cos_theta * z;
					vec3 temp{ newx, y, newz };
					for (int c = 0; c < 3; c++) {
						min[c] = fmin(min[c], temp[c]);
						max[c] = fmax(max[c], temp[c]);
					}
				}
}

std::optional<hit> rotate_y::hit_check(const ray& r, double t_min, double t_max) const {
	vec3 origin{
		cos_theta * r.origin()[0] - sin_theta * r.origin()[2],
		r.origin()[1],
		sin_theta * r.origin()[0] + cos_theta * r.origin()[2],
	};
	vec3 direction{
		cos_theta * r.direction()[0] - sin_theta * r.direction()[2],
		r.direction()[1],
		sin_theta * r.direction()[0] + cos_theta * r.direction()[2],
	};
	ray rotated_r{ origin, direction, r.time() };

	auto rec = p->hit_check(rotated_r, t_min, t_max);
	if (rec) {
		rec->point = vec3{
			cos_theta * rec->point[0] + sin_theta * rec->point[2],
			rec->point[1],
			-sin_theta * rec->point[0] + cos_theta * rec->point[2],
		};
		auto normal = vec3{
			cos_theta * rec->normal[0] + sin_theta * rec->normal[2],
			rec->normal[1],
			-sin_theta * rec->normal[0] + cos_theta * rec->normal[2],
		};
		rec->set_face_normal(rotated_r, normal);
	}
	return rec;
}