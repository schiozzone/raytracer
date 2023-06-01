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
#include "aarect.h"
#include "box.h"

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

visible_collection simple_light() {
	visible_collection objects;
	auto pertext = std::make_shared<lambertian>(std::make_shared<noise_texture>(4));
	objects.add(std::make_shared<sphere>(vec3{ 0, -1000, 0 }, 1000, pertext));
	objects.add(std::make_shared<sphere>(vec3{ 0, 2, 0 }, 2, pertext));
	auto difflight = std::make_shared<diffuse_light>(color(4, 4, 4));
	objects.add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
	auto nolight = std::make_shared<diffuse_light>(color(0, 0, 0));
	objects.add(std::make_shared<sphere>(vec3{ 0, 0, 0 }, -10000, nolight));
	return objects;
}

visible_collection cornell_box() {
	visible_collection objects;
	auto red = std::make_shared<lambertian>(color(.65, .05, .05));
	auto white = std::make_shared<lambertian>(color(.73, .73, .73));
	auto green = std::make_shared<lambertian>(color(.12, .45, .15));
	auto light = std::make_shared<diffuse_light>(color(15, 15, 15));
	auto background = std::make_shared<diffuse_light>(color(0, 0, 0));
	objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));

	objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));

	objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));
	objects.add(std::make_shared<sphere>(vec3{ 277, 277, 277 }, -2216, background));

	std::shared_ptr<visible> box1 = std::make_shared<box>(vec3{ 0, 0, 0 }, vec3{ 165, 330, 165 }, white);
	box1 = std::make_shared<rotate_y>(box1, 15);
	box1 = std::make_shared<translate>(box1, vec3{ 265, 0, 295 });
	std::shared_ptr<visible> box2 = std::make_shared<box>(vec3{ 0, 0, 0 }, vec3{ 165, 165, 165 }, white);
	box2 = std::make_shared<rotate_y>(box2, -18);
	box2 = std::make_shared<translate>(box2, vec3{ 130, 0, 65 });

	objects.add(box1);
	objects.add(box2);
	return objects;
}

color ray_color(const ray& r, const visible& world, int depth) {
	if (depth <= 0) return color(0, 0, 0);

	auto rec = world.hit_check(r, 0.001, infinity);
	if (rec) {
		auto scatter = rec->mat_ptr->scatter_check(r, rec.value());
		color emitted = rec->mat_ptr->emitted(rec->u, rec->v, rec->point);
		if (scatter) emitted += scatter->attenuation * ray_color(scatter->bounce, world, depth - 1);
		return emitted;
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
	constexpr double aspect_ratio = 1.0;// 16.0 / 9.0;
	constexpr int img_width = 400;// 600;
	const image img{ .width = img_width, .height = static_cast<int>(img_width / aspect_ratio) };
	constexpr int samples_per_pixel = 150;// 200;
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
	case 4:
		scene = earth();
		lookfrom = vec3(13, 2, 3);
		lookat = vec3(0, 0, 0);
		vfov = 20.0;
		break;
	case 5:
		scene = simple_light();
		lookfrom = vec3(26, 3, 6);
		lookat = vec3(0, 2, 0);
		vfov = 20.0;
		break;
	default:
	case 6:
		scene = cornell_box();
		lookfrom = vec3(278, 278, -800);
		lookat = vec3(278, 278, 0);
		vfov = 40.0;
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