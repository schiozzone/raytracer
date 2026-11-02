#pragma once

enum class SampleScene {
    random,
    two_spheres,
    two_perlin_spheres,
    earth,
    simple_light,
    cornell_box,
    cornell_smoke,
    final_scene,
};

void renderScene(SampleScene scn);