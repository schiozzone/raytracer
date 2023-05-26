#pragma once

#include "common.h"
#include "perlin.h"

class texture {
public:
	virtual color value(double u, double v, const vec3& p) const = 0;
};

class solid_color : public texture {
public:
	solid_color() {}
	solid_color(color c) : color_value(c) {}
	solid_color(double red, double green, double blue)
		: color_value(red, green, blue) {}

	color value(double u, double v, const vec3& p) const override {
		return color_value;
	}

private:
	color color_value;
};

class checker_texture : public texture {
public:
	checker_texture() {}
	checker_texture(std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
		: e(even), o(odd) {}
	checker_texture(color c1, color c2)
		: e(std::make_shared<solid_color>(c1)), o(std::make_shared<solid_color>(c2)) {}

	color value(double u, double v, const vec3& p) const override {
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
			return o->value(u, v, p);
		else
			return e->value(u, v, p);
	}

private:
	std::shared_ptr<texture> e;
	std::shared_ptr<texture> o;
};

class noise_texture : public texture {
public:
	noise_texture() : sc(1) {}
	noise_texture(double scale) : sc(scale) {}

	color value(double u, double v, const vec3& p) const override {
		return color(1, 1, 1) * 0.5 * (1 + sin(sc*p.z() + 10*noise.turb(p)));
	}

private:
	perlin noise;
	double sc;
};