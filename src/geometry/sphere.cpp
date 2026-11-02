#include "geometry/sphere.hpp"

#include "aabb.hpp"

std::optional<Hit> Sphere::hitCheck(const Ray& r, double t_min, double t_max) const {
	Vec3 o_c = r.origin() - this->c_;
	double a = r.direction().lengthSquared(); // <v, v> = |v|^2
	double hb = dot(r.direction(), o_c);
	double c = o_c.lengthSquared() - this->r_ * this->r_;
	double d = hb * hb - a * c;
	if (d < 0) { // at^2 + bt + c = 0 has no solutions
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
	Vec3 outward_normal = (rec.point - this->c_) / this->r_;
	rec.setFaceNormal(r, outward_normal);
	std::tie(rec.u, rec.v) = getSphereUv(outward_normal);
	rec.mat_ptr = this->m_;

	return rec;
}

std::optional<Aabb> Sphere::boundingBox(double /*time0*/, double /*time1*/) const {
	auto radius = fabs(this->r_);
	return Aabb(
	    this->c_ - Vec3(radius, radius, radius),
	    this->c_ + Vec3(radius, radius, radius)
	);
}
