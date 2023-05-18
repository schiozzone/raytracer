#pragma once

#include <cmath>
#include <limits>
#include <memory>

#include "ray.h"
#include "vec3.h"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double almost_one = std::nextafter(1.0, 0.0);

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}