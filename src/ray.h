#pragma once

#include "vec3.h"

class ray {
public:
	ray() {}
	ray(const vec3& origin, const vec3& direction, double time = 0.0)
		: A(origin), b(direction), tm(time) {}

	vec3 origin() const { return A; }
	vec3 direction() const { return b; }
	double time() const { return tm; }

	vec3 at(double t) const {
		return A + t * b;
	}

private:
	vec3 A, b;
	double tm;
};