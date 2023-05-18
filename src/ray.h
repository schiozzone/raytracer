#pragma once

#include "vec3.h"

class ray {
public:
	ray() {}
	ray(const vec3& origin, const vec3& direction)
		: A(origin), b(direction) {}

	vec3 origin() const { return A; }
	vec3 direction() const { return b; }

	vec3 at(double t) const {
		return A + t * b;
	}

private:
	vec3 A, b;
};