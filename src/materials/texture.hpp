#pragma once

#include <cstring>
#include <cassert>
#include <utility>

#include "math/common.hpp"
#include "materials/perlin.hpp"
#include "materials/stb_image.h"
#include "math/vec3.hpp"

class Texture {
public:
	virtual Color value(double u, double v, const Vec3& p) const = 0;
};

class SolidColor : public Texture {
public:
	SolidColor() {}
	SolidColor(Color c) : color_value_(c) {}
	SolidColor(double red, double green, double blue)
	    : color_value_(red, green, blue) {}

	Color value(double u, double v, const Vec3& p) const override {
		return color_value_;
	}

private:
	Color color_value_;
};

class CheckerTexture : public Texture {
public:
	CheckerTexture() {}
	CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
	    : e(std::move(even)), o(std::move(odd)) {}
	CheckerTexture(Color c1, Color c2)
	    : e(std::make_shared<SolidColor>(c1)), o(std::make_shared<SolidColor>(c2)) {}

	Color value(double u, double v, const Vec3& p) const override {
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
			return o->value(u, v, p);
	
		return e->value(u, v, p);
	}

private:
	std::shared_ptr<Texture> e;
	std::shared_ptr<Texture> o;
};

class NoiseTexture : public Texture {
public:
	NoiseTexture() : sc_(1) {}
	NoiseTexture(double scale) : sc_(scale) {}

	Color value(double u, double v, const Vec3& p) const override {
		return Color(1, 1, 1) * 0.5 * (1 + sin(sc_ * p.z() + 10 * noise_.turb(p)));
	}

private:
	Perlin noise_;
	double sc_;
};

class ImageTexture : public Texture {
public:
	ImageTexture() : width_(0), height_(0), bytes_per_pixel_(0) {}
	ImageTexture(const std::string& filename) {
		unsigned char* raw_data = stbi_load(filename.c_str(), &width_, &height_, &bytes_per_pixel_, 0);
		if (raw_data == NULL) {
			std::cerr << "ERROR: Could not load texture image file " << filename << ".\n";
			width_ = height_ = bytes_per_pixel_ = 0;
		}
		else {
			const size_t size = width_ * height_ * bytes_per_pixel_;
			data_.resize(size);
			std::memcpy(data_.data(), raw_data, size);
			stbi_image_free(raw_data);
		}
	}

	Color value(double u, double v, const Vec3& p) const override {
		if (data_.empty()) return Color(1, 0, 1);

		// Clamp UVs, could use wrapping instead
		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0); // Image coordinates go top to bottom

		auto i = static_cast<int>(u * width_);
		auto j = static_cast<int>(v * height_);
		// Should be at most equal, never greater
		if (i >= width_) i = width_ - 1;
		if (j >= height_) j = height_ - 1;

		const auto color_scale = 1.0 / 255.0;
		const auto bytes_per_scanline = width_ * bytes_per_pixel_;
		size_t pixel = j * bytes_per_scanline + i * bytes_per_pixel_;
		assert(bytes_per_pixel >= 3); // Less will break
		return Color(
		    color_scale * data_[pixel],
		    color_scale * data_[pixel + 1],
		    color_scale * data_[pixel + 2]
		);
	}

private:
	std::vector<unsigned char> data_;
	int width_, height_;
	int bytes_per_pixel_;
};
