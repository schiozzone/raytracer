#pragma once

#include <optional>

#include "common.h"
#include "visible.h"
#include "texture.h"

struct hit;

struct scatter {
	color attenuation;
	ray bounce;
};

class material {
public:
	virtual std::optional<scatter> scatter_check(const ray& r, const hit& rec) const = 0;
};

class lambertian : public material {
public:
	lambertian(const color& albedo) : a(std::make_shared<solid_color>(albedo)) {}
	lambertian(std::shared_ptr<texture> albedo) : a(albedo) {}

	std::optional<scatter> scatter_check(const ray& r, const hit& rec) const override {
		//auto scatter_direction = rec.normal + random_in_unit_sphere();
		auto scatter_direction = rec.normal + random_unit_vector();
		//auto scatter_direction = random_in_hemisphere(rec.normal);
		if (scatter_direction.near_zero()) scatter_direction = rec.normal; // Correct degenerate directions
		scatter s{};
		s.bounce = ray(rec.point, scatter_direction, r.time());
		s.attenuation = this->a->value(rec.u, rec.v, rec.point); // Could instead scatter with probabiliy p and have the atenuation be albedo/p
		return s;
	};

private:
	std::shared_ptr<texture> a;
};

class metal : public material {
public:
	metal(const color& albedo, double fuzz) : metal(std::make_shared<solid_color>(albedo), f) {}
	metal(std::shared_ptr<texture> albedo, double fuzz) : a(albedo), f(fuzz < 1 ? fuzz : 1) {}

	std::optional<scatter> scatter_check(const ray& r, const hit& rec) const override {
		vec3 reflected = reflect(unit_vector(r.direction()), rec.normal);
		scatter s{};
		s.bounce = ray(rec.point, reflected + f * random_in_unit_sphere(), r.time());
		s.attenuation = this->a->value(rec.u, rec.v, rec.point);
		if (dot(s.bounce.direction(), rec.normal) > 0) return s;
		return std::nullopt;
	};
private:
	std::shared_ptr<texture> a;
	double f;
};

class dielectric : public material {
public:
	dielectric(double refractive_index) : i(refractive_index) {}

	std::optional<scatter> scatter_check(const ray& r, const hit& rec) const override {
		double refraction_ratio = rec.front_face ? (1.0 / i) : i;

		vec3 unit_direction = unit_vector(r.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;
		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
			direction = reflect(unit_direction, rec.normal);
		}
		else {
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		scatter s{};
		s.bounce = ray(rec.point, direction, r.time());
		s.attenuation = color(1.0, 1.0, 1.0);
		return s;
	};
private:
	double i;

	static double reflectance(double cosine, double ref_idx) {
		// Schlick's
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};