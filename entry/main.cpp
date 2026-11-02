#include <iostream>
#include <assert.h>

#include "math/common.hpp"
#include "color.hpp"
#include "geometry/visible_collection.hpp"
#include "geometry/sphere.hpp"
#include "camera.hpp"
#include "math/random_number.hpp"
#include "materials/material.hpp"
#include "geometry/moving_sphere.hpp"
#include "bvh.hpp"
#include "geometry/aarect.hpp"
#include "geometry/box.hpp"
#include "geometry/constant_medium.hpp"

VisibleCollection randomScene() {
	VisibleCollection world;

	auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	world.add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(checker)));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = randomDouble();
			Vec3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

			if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = Color::random() * Color::random();
					sphere_material = std::make_shared<Lambertian>(albedo);
					auto center2 = center + Vec3(0, randomDouble(0, .5), 0);
					world.add(std::make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = Color::random(0.5, 1);
					auto fuzz = randomDouble(0, 0.5);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(1.5);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5);
	world.add(std::make_shared<Sphere>(Vec3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.add(std::make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<Sphere>(Vec3(4, 1, 0), 1.0, material3));

	return world;
}

VisibleCollection twoSpheres() {
	VisibleCollection objects;
	auto checker = std::make_shared<Lambertian>(
		std::make_shared<CheckerTexture>(
			Color(0.2, 0.3, 0.1),
			Color(0.9, 0.9, 0.9)
	));
	objects.add(std::make_shared<Sphere>(Vec3{ 0, -10, 0 }, 10, checker));
	objects.add(std::make_shared<Sphere>(Vec3{ 0, 10, 0 }, 10, checker));
	return objects;
}

VisibleCollection twoPerlinSpheres() {
	VisibleCollection objects;
	auto pertext = std::make_shared<Lambertian>(std::make_shared<NoiseTexture>(4));
	objects.add(std::make_shared<Sphere>(Vec3{ 0, -1000, 0 }, 1000, pertext));
	objects.add(std::make_shared<Sphere>(Vec3{ 0, 2, 0 }, 2, pertext));
	return objects;
}

VisibleCollection earth() {
	auto earth_texture = std::make_shared<ImageTexture>("Blue_Marble_2002.png");
	auto earth_surface = std::make_shared<Lambertian>(earth_texture);
	auto globe = std::make_shared<Sphere>(Vec3(0, 0, 0), 2, earth_surface);
	return VisibleCollection(globe);
}

VisibleCollection simpleLight() {
	VisibleCollection objects;
	auto pertext = std::make_shared<Lambertian>(std::make_shared<NoiseTexture>(4));
	objects.add(std::make_shared<Sphere>(Vec3{ 0, -1000, 0 }, 1000, pertext));
	objects.add(std::make_shared<Sphere>(Vec3{ 0, 2, 0 }, 2, pertext));
	auto difflight = std::make_shared<DiffuseLight>(Color(4, 4, 4));
	objects.add(std::make_shared<XyRect>(3, 5, 1, 3, -2, difflight));
	auto nolight = std::make_shared<DiffuseLight>(Color(0, 0, 0));
	objects.add(std::make_shared<Sphere>(Vec3{ 0, 0, 0 }, -10000, nolight));
	return objects;
}

VisibleCollection cornellBox() {
	VisibleCollection objects;
	auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
	auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
	auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));
	auto background = std::make_shared<DiffuseLight>(Color(0, 0, 0));
	objects.add(std::make_shared<YzRect>(0, 555, 0, 555, 555, green));
	objects.add(std::make_shared<YzRect>(0, 555, 0, 555, 0, red));

	objects.add(std::make_shared<XzRect>(213, 343, 227, 332, 554, light));
	objects.add(std::make_shared<XzRect>(0, 555, 0, 555, 0, white));
	objects.add(std::make_shared<XzRect>(0, 555, 0, 555, 555, white));

	objects.add(std::make_shared<XyRect>(0, 555, 0, 555, 555, white));
	objects.add(std::make_shared<Sphere>(Vec3{ 277, 277, 277 }, -2216, background));

	std::shared_ptr<Visible> box1 = std::make_shared<Box>(Vec3{ 0, 0, 0 }, Vec3{ 165, 330, 165 }, white);
	box1 = std::make_shared<RotateY>(box1, 15);
	box1 = std::make_shared<Translate>(box1, Vec3{ 265, 0, 295 });
	std::shared_ptr<Visible> box2 = std::make_shared<Box>(Vec3{ 0, 0, 0 }, Vec3{ 165, 165, 165 }, white);
	box2 = std::make_shared<RotateY>(box2, -18);
	box2 = std::make_shared<Translate>(box2, Vec3{ 130, 0, 65 });

	objects.add(box1);
	objects.add(box2);
	return objects;
}

VisibleCollection cornellSmoke() {
	VisibleCollection objects;
	auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
	auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
	auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
	auto background = std::make_shared<DiffuseLight>(Color(0, 0, 0));
	objects.add(std::make_shared<YzRect>(0, 555, 0, 555, 555, green));
	objects.add(std::make_shared<YzRect>(0, 555, 0, 555, 0, red));

	objects.add(std::make_shared<XzRect>(113, 443, 127, 432, 554, light));
	objects.add(std::make_shared<XzRect>(0, 555, 0, 555, 0, white));
	objects.add(std::make_shared<XzRect>(0, 555, 0, 555, 555, white));

	objects.add(std::make_shared<XyRect>(0, 555, 0, 555, 555, white));
	objects.add(std::make_shared<Sphere>(Vec3{ 277, 277, 277 }, -2216, background));

	std::shared_ptr<Visible> box1 = std::make_shared<Box>(Vec3{ 0, 0, 0 }, Vec3{ 165, 330, 165 }, white);
	box1 = std::make_shared<RotateY>(box1, 15);
	box1 = std::make_shared<Translate>(box1, Vec3{ 265, 0, 295 });
	std::shared_ptr<Visible> box2 = std::make_shared<Box>(Vec3{ 0, 0, 0 }, Vec3{ 165, 165, 165 }, white);
	box2 = std::make_shared<RotateY>(box2, -18);
	box2 = std::make_shared<Translate>(box2, Vec3{ 130, 0, 65 });

	objects.add(std::make_shared<ConstantMedium>(box1, 0.01, Color{ 0, 0, 0 }));
	objects.add(std::make_shared<ConstantMedium>(box2, 0.01, Color{ 1, 1, 1 }));
	return objects;
}

VisibleCollection finalScene() {
	VisibleCollection boxes1;
	auto ground = std::make_shared<Lambertian>(Color{ 0.48, 0.83, 0.53 });

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; ++i)
		for (int j = 0; j < boxes_per_side; ++j) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto z1 = z0 + w;
			auto y1 = randomDouble(1, 101);
			boxes1.add(std::make_shared<Box>(Vec3{ x0, y0, z0 }, Vec3{ x1, y1, z1 }, ground));
		}

	VisibleCollection objects;
	objects.add(std::make_shared<BvhNode>(boxes1, 0, 1));

	auto light = std::make_shared<DiffuseLight>(Color{ 7, 7, 7 });
	objects.add(std::make_shared<XzRect>(123, 423, 147, 412, 554, light));

	auto center1 = Vec3(400, 400, 200);
	auto center2 = center1 + Vec3(30, 0, 0);
	auto moving_sphere_material = std::make_shared<Lambertian>(Color{ 0.7, 0.3, 0.1 });
	objects.add(std::make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

	objects.add(std::make_shared<Sphere>(Vec3{ 260, 150, 45 }, 50, std::make_shared<Dielectric>(1.5)));
	objects.add(std::make_shared<Sphere>(Vec3{ 0, 150, 145 }, 50, std::make_shared<Metal>(Color{ 0.8, 0.8, 0.9 }, 1.0)));

	auto boundary = std::make_shared<Sphere>(Vec3{ 360, 150, 145 }, 70, std::make_shared<Dielectric>(1.5));
	objects.add(boundary);
	objects.add(std::make_shared<ConstantMedium>(boundary, 0.2, Color{ 0.2, 0.4, 0.9 }));

	objects.add(std::make_shared<ConstantMedium>(
		std::make_shared<Sphere>(Vec3{ 0, 0, 0 }, 5000, nullptr), 0.0001, Color{ 1, 1, 1 }));

	auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("Blue_Marble_2002.png"));
	objects.add(std::make_shared<Sphere>(Vec3{ 400, 200, 400 }, 100, emat));
	
	auto permat = std::make_shared<Lambertian>(std::make_shared<NoiseTexture>(0.1));
	objects.add(std::make_shared<Sphere>(Vec3{ 220, 280, 300 }, 80, permat));

	VisibleCollection boxes2;
	auto white = std::make_shared<Lambertian>(Color{ .73, .73, .73 });
	int ns = 1000;
	for (int j = 0; j < ns; j++)
		boxes2.add(std::make_shared<Sphere>(Vec3::random(0, 165), 10, white));
	objects.add(std::make_shared<Translate>(
		std::make_shared<RotateY>(
			std::make_shared<BvhNode>(boxes2, 0.0, 1.0),
			15
		),
		Vec3{ -100, 270, 395 }
	));

	return objects;
}

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
	return Color(0, 0, 0);
}

struct Image {
	uint64_t width, height;
};

// [Hacky multithreading support]
#include <thread>
#include <atomic>
#include <sstream>
std::atomic_int scanlines{ 0 };
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

int main() {
	assert((int)(256 * clamp(1, 0.0, almost_one)) == 255);

	// Render target
	constexpr double aspect_ratio = 1.0;// 16.0 / 9.0;
	constexpr int img_width = 800;
	const Image img{ .width = img_width, .height = static_cast<int>(img_width / aspect_ratio) };
	constexpr int samples_per_pixel = 100;// 10'000;
	constexpr int max_depth = 50;

	// Camera
	Vec3 lookfrom;
	Vec3 lookat;
	auto vfov = 40.0;
	auto aperture = 0.0;

	// Scene
	VisibleCollection scene;
	switch (0) {
	case 1:
		scene = randomScene();
		lookfrom = Vec3(13, 2, 3);
		lookat = Vec3(0, 0, 0);
		vfov = 20.0;
		aperture = 0.1;
		break;
	case 2:
		scene = twoSpheres();
		lookfrom = Vec3(13, 2, 3);
		lookat = Vec3(0, 0, 0);
		vfov = 20.0;
		break;
	case 3:
		scene = twoPerlinSpheres();
		lookfrom = Vec3(13, 2, 3);
		lookat = Vec3(0, 0, 0);
		vfov = 20.0;
		break;
	case 4:
		scene = earth();
		lookfrom = Vec3(13, 2, 3);
		lookat = Vec3(0, 0, 0);
		vfov = 20.0;
		break;
	case 5:
		scene = simpleLight();
		lookfrom = Vec3(26, 3, 6);
		lookat = Vec3(0, 2, 0);
		vfov = 20.0;
		break;
	case 6:
		scene = cornellBox();
		lookfrom = Vec3(278, 278, -800);
		lookat = Vec3(278, 278, 0);
		vfov = 40.0;
		break;
	case 7:
		scene = cornellSmoke();
		lookfrom = Vec3(278, 278, -800);
		lookat = Vec3(278, 278, 0);
		vfov = 40.0;
		break;
	default:
	case 8:
		scene = finalScene();
		lookfrom = Vec3(478, 278, -600);
		lookat = Vec3(278, 278, 0);
		vfov = 40.0;
		break;
	}
	BvhNode world(scene, 0.0, 1.0);

	// Camera
	Vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

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
				pixel += ray_color(r, world, max_depth);
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
	return 0;
}