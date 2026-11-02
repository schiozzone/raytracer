#pragma once

#include <cassert>
#include <utility>

#include "geometry/visible.hpp"

class XyRect : public Visible {
public:
	XyRect() : x0_(0), x1_(0), y0_(0), y1_(0), k_(0) {}
	XyRect(double x0, double x1, double y0, double y1, double z, std::shared_ptr<Material> material)
	    : m_(std::move(material)), x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(z) {
		assert(x0 <= x1);
		assert(y0 <= y1);
	};

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override;
	std::optional<Aabb> boundingBox(double time0, double time1) const override;

private:
	std::shared_ptr<Material> m_;
	double x0_, x1_, y0_, y1_, k_;
};

class XzRect : public Visible {
public:
	XzRect() : x0_(0), x1_(0), z0_(0), z1_(0), k_(0) {}
	XzRect(double x0, double x1, double z0, double z1, double y, std::shared_ptr<Material> material)
	    : m_(std::move(material)), x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(y) {
		assert(x0 <= x1);
		assert(z0 <= z1);
	};

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override;
	std::optional<Aabb> boundingBox(double time0, double time1) const override;

private:
	std::shared_ptr<Material> m_;
	double x0_, x1_, z0_, z1_, k_;
};

class YzRect : public Visible {
public:
	YzRect() : y0_(0), y1_(0), z0_(0), z1_(0), k_(0) {}
	YzRect(double y0, double y1, double z0, double z1, double x, std::shared_ptr<Material> material)
	    : m_(std::move(material)), y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(x) {
		assert(y0 <= y1);
		assert(z0 <= z1);
	};

	std::optional<Hit> hitCheck(const Ray& r, double t_min, double t_max) const override;
	std::optional<Aabb> boundingBox(double time0, double time1) const override;

private:
	std::shared_ptr<Material> m_;
	double y0_, y1_, z0_, z1_, k_;
};
