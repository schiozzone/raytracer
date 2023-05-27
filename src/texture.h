#pragma once

#include "common.h"
#include "perlin.h"
#include "stb_image.h"

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

class image_texture : public texture {
public:
	image_texture() : data(), width(0), height(0), bytes_per_pixel(0) {}
	image_texture(const std::string& filename) {
		unsigned char* raw_data = stbi_load(filename.c_str(), &width, &height, &bytes_per_pixel, 0);
		if (raw_data == NULL) {
			std::cerr << "ERROR: Could not load texture image file " << filename << ".\n";
			width = height = bytes_per_pixel = 0;
		}
		else {
			const size_t size = width * height * bytes_per_pixel;
			data.resize(size);
			memcpy_s(data.data(), data.size(), raw_data, size);
			stbi_image_free(raw_data);
		}
	}

	color value(double u, double v, const vec3& p) const override {
		if (data.empty()) return color(1, 0, 1);

		// Clamp UVs, could use wrapping instead
		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0); // Image coordinates go top to bottom

		auto i = static_cast<int>(u * width);
		auto j = static_cast<int>(v * height);
		// Should be at most equal, never greater
		if (i >= width) i = width - 1;
		if (j >= height) j = height - 1;

		const auto color_scale = 1.0 / 255.0;
		const auto bytes_per_scanline = width * bytes_per_pixel;
		size_t pixel = j * bytes_per_scanline + i * bytes_per_pixel;
		assert(bytes_per_pixel >= 3); // Less will break
		return color(
			color_scale * data[pixel],
			color_scale * data[pixel + 1],
			color_scale * data[pixel + 2]
		);
	}

private:
	std::vector<unsigned char> data;
	int width, height;
	int bytes_per_pixel;
};