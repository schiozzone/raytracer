
#include "render.hpp"

#include <iostream>

#include "math/common.hpp"
#include "color.hpp"

#include "math/random_number.hpp"
#include "materials/material.hpp"

Color rayColor(const Ray& r, const Visible& world, int depth) {
	if (depth <= 0) return Color(0, 0, 0);

	auto rec = world.hitCheck(r, 0.001, infinity);
	if (rec) {
		auto scatter = rec->mat_ptr->scatterCheck(r, rec.value());
		Color emitted = rec->mat_ptr->emitted(rec->u, rec->v, rec->point);
		if (scatter) emitted += scatter->attenuation * rayColor(scatter->bounce, world, depth - 1);
		return emitted;
	}

	/*Vec3 unit_direction = unitVector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);*/
	return {0, 0, 0};
}

// [Hacky multithreading support]
#include <thread>
#include <atomic>
#include <sstream>
static std::atomic_int scanlines{ 0 };
void tFunc(
    int start, int end, Color* data,
    int height, int width, int samples_per_pixel,
    const Camera& cam, const BvhNode& world, int max_depth
) {
	for (int j = start; j < end; ++j) {
		for (int i = 0; i < width; ++i) {
			int k = height - 1 - j; // Top to bottom
			Color pixel(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + randomDouble()) / (width - 1);
				auto v = (k + randomDouble()) / (height - 1);
				Ray r = cam.getRay(u, v);
				pixel += rayColor(r, world, max_depth);
			}
			data[j * width + i] = pixel;
		}
		int print_val = ++scanlines;
		std::stringstream msg;
		msg << "\rScanlines remaining: " << (height - print_val) << ' ';
		std::cerr << msg.str();
	}
}
// [/Hacky multithreading support]

void render(Image img, int samples_per_pixel, const Camera& cam, const BvhNode& world, int max_depth) {
	// [Hacky multithreading support]
#if 0
	std::cout << "P3\n" << img.width << ' ' << img.height << "\n255\n";
	for (int j = img.height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < img.width; ++i) {
			Color pixel(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + randomDouble()) / (img.width - 1);
				auto v = (j + randomDouble()) / (img.height - 1);
				Ray r = cam.getRay(u, v);
				pixel += rayColor(r, world, max_depth);
			}
			writeColor(std::cout, pixel, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
#else
	std::vector<Color> framebuffer;
	framebuffer.resize(img.width * img.height);
	std::vector<std::thread> threads;
	int cores = std::thread::hardware_concurrency();
	std::cerr << "Found " << cores << " cores.\n" << std::flush;
	const int num_threads = cores * 2;
	int lines_per_thread = img.height / num_threads;

	threads.reserve(num_threads);
    for (int i = 0; i < num_threads - 1; ++i)
		threads.emplace_back(tFunc, i * lines_per_thread, (i + 1) * lines_per_thread, framebuffer.data(), img.height, img.width, samples_per_pixel, cam, world, max_depth);
	threads.emplace_back(tFunc, (num_threads - 1) * lines_per_thread, img.height, framebuffer.data(), img.height, img.width, samples_per_pixel, cam, world, max_depth);

	for (std::thread& t : threads)
		t.join();

	std::cout << "P3\n" << img.width << ' ' << img.height << "\n255\n";
	std::cerr << "\nWriting image...\n";
	for(const auto& pixel : framebuffer)
		writeColor(std::cout, pixel, samples_per_pixel);
	std::cerr << "\nDone.\n";
#endif
	// [/Hacky multithreading support]
}