#pragma once

#include "math/common.hpp"
#include "math/ray.hpp"
#include "math/vec3.hpp"

class Camera {
public:
	Camera(
	    Vec3 lookfrom,
	    Vec3 lookat,
	    Vec3 vup,
	    double vfov,
	    double aspect_ratio,
	    double aperture,
	    double focus_dist,
	    double time0 = 0,
	    double time1 = 0
	) : origin_(lookfrom), lens_radius_(aperture / 2), t0_(time0), t1_(time1) {
		auto theta = degreesToRadians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		w_ = unitVector(lookfrom - lookat);
		u_ = unitVector(cross(vup, w_));
		v_ = cross(w_, u_);

		horizontal_ = focus_dist * viewport_width * u_;
		vertical_ = focus_dist * viewport_height * v_;
		lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - focus_dist * w_;
	}

	Ray getRay(double s, double t) const {
		Vec3 rd = lens_radius_ * randomInUnitDisk();
		Vec3 offset = u_ * rd.x() + v_ * rd.y();
		return Ray(
		    origin_ + offset,
		    lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset,
		    randomDouble(t0_, t1_)
		);
	}

private:
	Vec3 origin_, horizontal_, vertical_, lower_left_corner_;
	Vec3 u_, v_, w_;
	double lens_radius_, t0_, t1_;
};
