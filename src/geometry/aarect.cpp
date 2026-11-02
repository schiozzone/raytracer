#include "geometry/aarect.hpp"

#include "aabb.hpp"

std::optional<Hit> XyRect::hitCheck(const Ray& r, double t_min, double t_max) const {
	auto t = (this->k_ - r.origin().z()) / r.direction().z();
	if (t < t_min || t > t_max)
		return std::nullopt;
	auto x = r.origin().x() + t * r.direction().x();
	auto y = r.origin().y() + t * r.direction().y();
	if (x < this->x0_ || x > this->x1_ || y < this->y0_ || y > this->y1_)
		return std::nullopt;

	Hit rec;
	rec.t = t;
	rec.point = r.at(t);
	Vec3 outward_normal = Vec3(0, 0, 1);
	rec.setFaceNormal(r, outward_normal);
	rec.u = (x - this->x0_) / (this->x1_ - this->x0_);
	rec.v = (y - this->y0_) / (this->y1_ - this->y0_);
	rec.mat_ptr = this->m_;

	return rec;
}

std::optional<Aabb> XyRect::boundingBox(double /*time0*/, double /*time1*/) const {
	// Adds some padding on the z dimension
	return Aabb(
	    Vec3(x0_, y0_, k_ - 0.0001),
	    Vec3(x1_, y1_, k_ + 0.0001)
	);
}

std::optional<Hit> XzRect::hitCheck(const Ray& r, double t_min, double t_max) const {
	auto t = (this->k_ - r.origin().y()) / r.direction().y();
	if (t < t_min || t > t_max)
		return std::nullopt;
	auto x = r.origin().x() + t * r.direction().x();
	auto z = r.origin().z() + t * r.direction().z();
	if (x < this->x0_ || x > this->x1_ || z < this->z0_ || z > this->z1_)
		return std::nullopt;

	Hit rec;
	rec.t = t;
	rec.point = r.at(t);
	Vec3 outward_normal = Vec3(0, 1, 0);
	rec.setFaceNormal(r, outward_normal);
	rec.u = (x - this->x0_) / (this->x1_ - this->x0_);
	rec.v = (z - this->z0_) / (this->z1_ - this->z0_);
	rec.mat_ptr = this->m_;

	return rec;
}

std::optional<Aabb> XzRect::boundingBox(double /*time0*/, double /*time1*/) const {
	// Adds some padding on the y dimension
	return Aabb(
	    Vec3(x0_, k_ - 0.0001, z0_),
	    Vec3(x1_, k_ + 0.0001, z1_)
	);
}

std::optional<Hit> YzRect::hitCheck(const Ray& r, double t_min, double t_max) const {
	auto t = (this->k_ - r.origin().x()) / r.direction().x();
	if (t < t_min || t > t_max)
		return std::nullopt;
	auto y = r.origin().y() + t * r.direction().y();
	auto z = r.origin().z() + t * r.direction().z();
	if (y < this->y0_ || y > this->y1_ || z < this->z0_ || z > this->z1_)
		return std::nullopt;

	Hit rec;
	rec.t = t;
	rec.point = r.at(t);
	Vec3 outward_normal = Vec3(1, 0, 0);
	rec.setFaceNormal(r, outward_normal);
	rec.u = (y - this->y0_) / (this->y1_ - this->y0_);
	rec.v = (z - this->z0_) / (this->z1_ - this->z0_);
	rec.mat_ptr = this->m_;

	return rec;
}

std::optional<Aabb> YzRect::boundingBox(double /*time0*/, double /*time1*/) const {
	// Adds some padding on the x dimension
	return Aabb(
	    Vec3(k_ - 0.0001, y0_, z0_),
	    Vec3(k_ + 0.0001, y1_, z1_)
	);
}
