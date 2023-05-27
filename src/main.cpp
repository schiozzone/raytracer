#include <iostream>
#include <assert.h>

#include "common.h"
#include "color.h"
#include "visible_collection.h"
#include "sphere.h"
#include "camera.h"
#include "random_number.h"
#include "material.h"
#include "moving_sphere.h"
#include "bvh.h"

visible_collection random_scene() {
	visible_collection world;

	auto checker = std::make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	world.add(std::make_shared<sphere>(vec3(0, -1000, 0), 1000, std::make_shared<lambertian>(checker)));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = std::make_shared<lambertian>(albedo);
					auto center2 = center + vec3(0, random_double(0, .5), 0);
					world.add(std::make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<metal>(albedo, fuzz);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<dielectric>(1.5);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<dielectric>(1.5);
	world.add(std::make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(std::make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));

	return world;
}

visible_collection two_spheres() {
	visible_collection objects;
	auto checker = std::make_shared<lambertian>(
		std::make_shared<checker_texture>(
			color(0.2, 0.3, 0.1),
			color(0.9, 0.9, 0.9)
	));
	objects.add(std::make_shared<sphere>(vec3{ 0, -10, 0 }, 10, checker));
	objects.add(std::make_shared<sphere>(vec3{ 0, 10, 0 }, 10, checker));
	return objects;
}

visible_collection two_perlin_spheres() {
	visible_collection objects;
	auto pertext = std::make_shared<lambertian>(std::make_shared<noise_texture>(4));
	objects.add(std::make_shared<sphere>(vec3{ 0, -1000, 0 }, 1000, pertext));
	objects.add(std::make_shared<sphere>(vec3{ 0, 2, 0 }, 2, pertext));
	return objects;
}

visible_collection earth() {
	auto earth_texture = std::make_shared<image_texture>("Blue_Marble_2002.png");
	auto earth_surface = std::make_shared<lambertian>(earth_texture);
	auto globe = std::make_shared<sphere>(vec3(0, 0, 0), 2, earth_surface);
	return visible_collection(globe);
}

color ray_color(const ray& r, const visible& world, int depth) {
	if (depth <= 0) return color(0, 0, 0);

	auto rec = world.hit_check(r, 0.001, infinity);
	if (rec) {
		auto scatter = rec->mat_ptr->scatter_check(r, rec.value());
		if (scatter) return scatter->attenuation * ray_color(scatter->bounce, world, depth - 1);
		return color(0, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

struct image {
	uint64_t width, height;
};

int main(int argc, char* argv) {
	assert((int)(256 * clamp(1, 0.0, almost_one)) == 255);

	// Render target
	constexpr double aspect_ratio = 16.0 / 9.0;
	constexpr int img_width = 400;
	const image img{ .width = img_width, .height = static_cast<int>(img_width / aspect_ratio) };
	constexpr int samples_per_pixel = 100;
	constexpr int max_depth = 50;

	// Camera
	vec3 lookfrom;
	vec3 lookat;
	auto vfov = 40.0;
	auto aperture = 0.0;

	// Scene
	visible_collection scene;
	switch (0) {
	case 1:
		scene = random_scene();
		lookfrom = vec3(13, 2, 3);
		lookat = vec3(0, 0, 0);
		vfov = 20.0;
		aperture = 0.1;
		break;
	case 2:
		scene = two_spheres();
		lookfrom = vec3(13, 2, 3);
		lookat = vec3(0, 0, 0);
		vfov = 20.0;
		break;
	case 3:
		scene = two_perlin_spheres();
		lookfrom = vec3(13, 2, 3);
		lookat = vec3(0, 0, 0);
		vfov = 20.0;
		break;
	default:
	case 4:
		scene = earth();
		lookfrom = vec3(13, 2, 3);
		lookat = vec3(0, 0, 0);
		vfov = 20.0;
		break;
	}
	bvh_node world(scene, 0.0, 1.0);

	// Camera
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	std::cout << "P3\n" << img.width << ' ' << img.height << "\n255\n";
	for (int j = img.height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < img.width; ++i) {
			color pixel(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (img.width - 1);
				auto v = (j + random_double()) / (img.height - 1);
				ray r = cam.get_ray(u, v);
				pixel += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
	return 0;
}