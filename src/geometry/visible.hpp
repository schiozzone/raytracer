#pragma once

#include <memory>
#include <optional>
#include <utility>

#include "math/ray.hpp"
#include "aabb.hpp"

class Material;

struct Hit {
	Vec3 point;
	Vec3 normal;
	std::shared_ptr<Material> mat_ptr;
	double t;
	double u;
	double v;
	bool front_face;

	 void setFaceNormal(const Ray& r, const Vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Visible {
public:
	virtual std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const = 0;
	virtual std::optional<Aabb> boundingBox(double time0, double time1) const = 0;
};

class Translate : public Visible {
public:
	Translate(std::shared_ptr<Visible> primitive, const Vec3& displacement)
	    : p_(std::move(primitive)), offset_(displacement) {}

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override;
	std::optional<Aabb> boundingBox(double time0, double time1) const override;

private:
	std::shared_ptr<Visible> p_;
	Vec3 offset_;
};

class RotateY : public Visible {
public:
	RotateY(std::shared_ptr<Visible> primitive, double angle);

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override;
	std::optional<Aabb> boundingBox(double /*time0*/, double /*time1*/) const override {
		return bbox_;
	}

private:
	std::shared_ptr<Visible> p_;
	double sin_theta_;
	double cos_theta_;
	std::optional<Aabb> bbox_;
};
