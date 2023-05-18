#pragma once

#include <iostream>

#include "vec3.h"
#include "common.h"

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	auto scale = 1.0 / samples_per_pixel;
	r = std::sqrt(scale * r);
	g = std::sqrt(scale * g);
	b = std::sqrt(scale * b);

	out << static_cast<int>(256 * clamp(r, 0.0, almost_one)) << '\t'
		<< static_cast<int>(256 * clamp(g, 0.0, almost_one)) << '\t'
		<< static_cast<int>(256 * clamp(b, 0.0, almost_one)) << '\n';
}
