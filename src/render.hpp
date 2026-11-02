#pragma once

#include "camera.hpp"
#include "bvh.hpp"

struct Image {
	uint64_t width, height;
};

void render(Image img, int samples_per_pixel, const Camera& cam, const BvhNode& world, int max_depth);