#include <raytracer/raytracer.hpp>

#include <cassert>
#include "render.hpp"
#include "scene.hpp"

void renderScene(SampleScene scn) {
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
	switch (scn) {
	case SampleScene::random:
		scene = randomScene();
		lookfrom = Vec3(13, 2, 3);
		lookat = Vec3(0, 0, 0);
		vfov = 20.0;
		aperture = 0.1;
		break;
	case SampleScene::two_spheres:
		scene = twoSpheres();
		lookfrom = Vec3(13, 2, 3);
		lookat = Vec3(0, 0, 0);
		vfov = 20.0;
		break;
	case SampleScene::two_perlin_spheres:
		scene = twoPerlinSpheres();
		lookfrom = Vec3(13, 2, 3);
		lookat = Vec3(0, 0, 0);
		vfov = 20.0;
		break;
	case SampleScene::earth:
		scene = earth();
		lookfrom = Vec3(13, 2, 3);
		lookat = Vec3(0, 0, 0);
		vfov = 20.0;
		break;
	case SampleScene::simple_light:
		scene = simpleLight();
		lookfrom = Vec3(26, 3, 6);
		lookat = Vec3(0, 2, 0);
		vfov = 20.0;
		break;
	case SampleScene::cornell_box:
		scene = cornellBox();
		lookfrom = Vec3(278, 278, -800);
		lookat = Vec3(278, 278, 0);
		vfov = 40.0;
		break;
	case SampleScene::cornell_smoke:
		scene = cornellSmoke();
		lookfrom = Vec3(278, 278, -800);
		lookat = Vec3(278, 278, 0);
		vfov = 40.0;
		break;
	default:
	case SampleScene::final_scene:
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

	render(img, samples_per_pixel, cam, world, max_depth);
}