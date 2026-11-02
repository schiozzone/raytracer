#include "geometry/constant_medium.hpp"

std::optional<Hit> ConstantMedium::hitCheck(const Ray& r, double t_min, double t_max) const {
	auto rec1 = convex_->hitCheck(r, -infinity, infinity);
	if (!rec1) return std::nullopt;
	auto rec2 = convex_->hitCheck(r, rec1->t + 0.0001, infinity);
	if (!rec2) return std::nullopt;

	if (rec1->t < t_min) rec1->t = t_min;
	if (rec2->t > t_max) rec2->t = t_max;
	if (rec1->t < 0) rec1->t = 0;
	if (rec1->t >= rec2->t) return std::nullopt;

	const auto ray_length = r.direction().length();
	const auto distance_inside_boundary = (rec2->t - rec1->t) * ray_length;
	const auto hit_distance = neg_inv_density_ * log(randomDouble());
	if (hit_distance > distance_inside_boundary) return std::nullopt;

	Hit rec;
	rec.t = rec1->t + hit_distance / ray_length;
	rec.point = r.at(rec.t);
	rec.normal = Vec3{ 1, 0, 0 }; // random_in_unit_sphere();
	rec.front_face = true;        // Arbitrary
	rec.mat_ptr = phase_function_;
	return rec;
}
