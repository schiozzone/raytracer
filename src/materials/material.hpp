#pragma once

#include <optional>
#include <utility>

#include "geometry/visible.hpp"
#include "materials/texture.hpp"

struct Scatter {
	Color attenuation;
	Ray bounce;
};

class Material {
public:
	virtual std::optional<Scatter> scatterCheck(const Ray& r, const Hit& rec) const = 0;
	virtual Color emitted(double u, double v, const Vec3& p) const { return { 0, 0, 0 }; }
};

class Lambertian : public Material {
public:
	Lambertian(const Color& albedo) : a_(std::make_shared<SolidColor>(albedo)) {}
	Lambertian(std::shared_ptr<Texture> albedo) : a_(std::move(albedo)) {}

	std::optional<Scatter> scatterCheck(const Ray& r, const Hit& rec) const override {
		// auto scatter_direction = rec.normal + random_in_unit_sphere();
		auto scatter_direction = rec.normal + randomUnitVector();
		// auto scatter_direction = random_in_hemisphere(rec.normal);
		if (scatter_direction.nearZero()) scatter_direction = rec.normal; // Correct degenerate directions
		Scatter s{};
		s.bounce = Ray(rec.point, scatter_direction, r.time());
		s.attenuation = this->a_->value(rec.u, rec.v, rec.point); // Could instead scatter with probabiliy p and have the atenuation be albedo/p
		return s;
	};

private:
	std::shared_ptr<Texture> a_;
};

class Metal : public Material {
public:
	Metal(const Color& albedo, double fuzz) : Metal(std::make_shared<SolidColor>(albedo), fuzz) {}
	Metal(std::shared_ptr<Texture> albedo, double fuzz) : a_(std::move(albedo)), f_(fuzz < 1 ? fuzz : 1) {}

	std::optional<Scatter> scatterCheck(const Ray& r, const Hit& rec) const override {
		Vec3 reflected = reflect(unitVector(r.direction()), rec.normal);
		Scatter s{};
		s.bounce = Ray(rec.point, reflected + f_ * randomInUnitSphere(), r.time());
		s.attenuation = this->a_->value(rec.u, rec.v, rec.point);
		if (dot(s.bounce.direction(), rec.normal) > 0) return s;
		return std::nullopt;
	};

private:
	std::shared_ptr<Texture> a_;
	double f_;
};

class Dielectric : public Material {
public:
	Dielectric(double refractive_index) : i_(refractive_index) {}

	std::optional<Scatter> scatterCheck(const Ray& r, const Hit& rec) const override {
		double refraction_ratio = rec.front_face ? (1.0 / i_) : i_;

		Vec3 unit_direction = unitVector(r.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		Vec3 direction;
		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > randomDouble()) {
			direction = reflect(unit_direction, rec.normal);
		}
		else {
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		Scatter s{};
		s.bounce = Ray(rec.point, direction, r.time());
		s.attenuation = Color(1.0, 1.0, 1.0);
		return s;
	};

private:
	double i_;

	static double reflectance(double cosine, double ref_idx) {
		// Schlick's
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

class DiffuseLight : public Material {
public:
	DiffuseLight(const Color& emit) : e_(std::make_shared<SolidColor>(emit)) {}
	DiffuseLight(std::shared_ptr<Texture> emit) : e_(std::move(emit)) {}

	std::optional<Scatter> scatterCheck(const Ray& r, const Hit& rec) const override {
		return std::nullopt;
	};

	Color emitted(double u, double v, const Vec3& p) const override {
		return e_->value(u, v, p);
	}

private:
	std::shared_ptr<Texture> e_;
};

class Isotropic : public Material {
public:
	Isotropic(Color c) : a_(std::make_shared<SolidColor>(c)) {}
	Isotropic(std::shared_ptr<Texture> albedo) : a_(std::move(albedo)) {}

	std::optional<Scatter> scatterCheck(const Ray& r, const Hit& rec) const override {
		Scatter s{};
		s.bounce = Ray(rec.point, randomInUnitSphere(), r.time());
		s.attenuation = a_->value(rec.u, rec.v, rec.point);
		return s;
	};

private:
	std::shared_ptr<Texture> a_;
};
