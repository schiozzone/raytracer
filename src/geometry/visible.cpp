#include <utility>

#include "geometry/visible.hpp"

#include "aabb.hpp"
#include "math/common.hpp"

std::optional<Hit> Translate::hitCheck(const Ray& r, double t_min, double t_max) const {
	Ray offset_r{ r.origin() - offset_, r.direction(), r.time() };
	auto rec = p_->hitCheck(offset_r, t_min, t_max);
	if (rec) {
		rec->point += offset_;
		rec->setFaceNormal(offset_r, rec->normal);
	}
	return rec;
}

std::optional<Aabb> Translate::boundingBox(double time0, double time1) const {
	auto output_box = p_->boundingBox(time0, time1);
	if (output_box) {
		output_box.emplace(
		    output_box->min() + offset_,
		    output_box->max() + offset_
		);
	}
	return output_box;
}

RotateY::RotateY(std::shared_ptr<Visible> primitive, double angle) : p_(std::move(primitive)) {
	auto radians = degreesToRadians(angle);
	sin_theta_ = sin(radians);
	cos_theta_ = cos(radians);
	bbox_ = p_->boundingBox(0, 1); // Fixed time interval

	Vec3 min{ infinity, infinity, infinity };
	Vec3 max{ -infinity, -infinity, -infinity };

	if (bbox_)
		for (auto x : { bbox_->min().x(), bbox_->max().x() })
			for (auto y : { bbox_->min().y(), bbox_->max().y() })
				for (auto z : { bbox_->min().z(), bbox_->max().z() }) {
					auto newx = cos_theta_ * x + sin_theta_ * z;
					auto newz = -sin_theta_ * x + cos_theta_ * z;
					Vec3 temp{ newx, y, newz };
					for (int c = 0; c < 3; c++) {
						min[c] = fmin(min[c], temp[c]);
						max[c] = fmax(max[c], temp[c]);
					}
				}
}

std::optional<Hit> RotateY::hitCheck(const Ray& r, double t_min, double t_max) const {
	Vec3 origin{
		cos_theta_ * r.origin()[0] - sin_theta_ * r.origin()[2],
		r.origin()[1],
		sin_theta_ * r.origin()[0] + cos_theta_ * r.origin()[2],
	};
	Vec3 direction{
		cos_theta_ * r.direction()[0] - sin_theta_ * r.direction()[2],
		r.direction()[1],
		sin_theta_ * r.direction()[0] + cos_theta_ * r.direction()[2],
	};
	Ray rotated_r{ origin, direction, r.time() };

	auto rec = p_->hitCheck(rotated_r, t_min, t_max);
	if (rec) {
		rec->point = Vec3{
			cos_theta_ * rec->point[0] + sin_theta_ * rec->point[2],
			rec->point[1],
			-sin_theta_ * rec->point[0] + cos_theta_ * rec->point[2],
		};
		auto normal = Vec3{
			cos_theta_ * rec->normal[0] + sin_theta_ * rec->normal[2],
			rec->normal[1],
			-sin_theta_ * rec->normal[0] + cos_theta_ * rec->normal[2],
		};
		rec->setFaceNormal(rotated_r, normal);
	}
	return rec;
}
