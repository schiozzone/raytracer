#pragma once

#include "math/vec3.hpp"

class Ray {
public:
	Ray() = default;
	Ray(const Vec3& origin, const Vec3& direction, double time = 0.0)
	    : a_(origin), b_(direction), tm_(time) {}

	Vec3 origin() const { return a_; }
	Vec3 direction() const { return b_; }
	double time() const { return tm_; }

	Vec3 at(double t) const {
		return a_ + t * b_;
	}

private:
	Vec3 a_, b_;
	double tm_;
};
