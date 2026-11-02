#include "scene.hpp"

#include "geometry/sphere.hpp"
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
