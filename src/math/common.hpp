#pragma once

#include "math/random_number.hpp"
#include <cmath>
#include <limits>


const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double almost_one = std::nextafter(1.0, 0.0);

inline double degreesToRadians(double degrees) {
	return degrees * pi / 180.0;
}

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

inline int randomInt(int min, int max) {
	return static_cast<int>(randomDouble(min, max + 1));
}
