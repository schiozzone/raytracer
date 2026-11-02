#include "geometry/moving_sphere.hpp"

#include "geometry/sphere.hpp"
#include "aabb.hpp"

Vec3 MovingSphere::center(double time) const {
	return c0_ + (c1_ - c0_) * ((time - t0_) / (t1_ - t0_));
}

std::optional<Hit> MovingSphere::hitCheck(const Ray& r, double t_min, double t_max) const {
	Vec3 o_c = r.origin() - center(r.time()); // Changed from sphere code
	double a = r.direction().lengthSquared();
	double hb = dot(r.direction(), o_c);
	double c = o_c.lengthSquared() - this->r_ * this->r_;
	double d = hb * hb - a * c;
	if (d < 0) {
		return std::nullopt;
	}

	double d_sqrt = std::sqrt(d);
	double root = (-hb - d_sqrt) / a;
	if (root < t_min || root > t_max) {
		root = (-hb + d_sqrt) / a;
		if (root < t_min || root > t_max) return std::nullopt;
	}

	Hit rec;
	rec.t = root;
	rec.point = r.at(rec.t);
	Vec3 outward_normal = (rec.point - center(r.time())) / this->r_; // Changed from sphere code
	rec.setFaceNormal(r, outward_normal);
	std::tie(rec.u, rec.v) = Sphere::getSphereUv(outward_normal);
	rec.mat_ptr = this->m_;

	return rec;
}

std::optional<Aabb> MovingSphere::boundingBox(double /*time0*/, double /*time1*/) const {
	auto radius = fabs(this->r_);
	return surroundingBox(
	    Aabb(
	        this->c0_ - Vec3(radius, radius, radius),
	        this->c0_ + Vec3(radius, radius, radius)
	    ),
	    Aabb(
	        this->c1_ - Vec3(radius, radius, radius),
	        this->c1_ + Vec3(radius, radius, radius)
	    )
	);
}
