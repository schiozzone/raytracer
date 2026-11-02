#pragma once

#include <cmath>
#include <iostream>

#include "math/random_number.hpp"

class Vec3 {
public:
	Vec3() : v_{ 0, 0, 0 } {}
	Vec3(double v0, double v1, double v2) : v_{ v0, v1, v2 } {}

	double x() const { return v_[0]; }
	double y() const { return v_[1]; }
	double z() const { return v_[2]; }

	Vec3 operator-() const { return Vec3(-v_[0], -v_[1], -v_[2]); }
	double operator[](int i) const { return v_[i]; }
	double& operator[](int i) { return v_[i]; }

	Vec3& operator+=(const Vec3& a) {
		v_[0] += a.v_[0];
		v_[1] += a.v_[1];
		v_[2] += a.v_[2];
		return *this;
	}

	Vec3& operator*=(const double t) {
		v_[0] *= t;
		v_[1] *= t;
		v_[2] *= t;
		return *this;
	}

	Vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	double length() const {
		return std::sqrt(lengthSquared());
	}

	double lengthSquared() const {
		return v_[0] * v_[0] + v_[1] * v_[1] + v_[2] * v_[2];
	}

	bool nearZero() const {
		constexpr auto epsilon = 1e-8;
		return (fabs(v_[0]) < epsilon) && (fabs(v_[1]) < epsilon) && (fabs(v_[2]) < epsilon);
	}

private:
	double v_[3];

public:
	friend std::ostream& operator<<(std::ostream& out, const Vec3& a) {
		return out << a.v_[0] << ' ' << a.v_[1] << ' ' << a.v_[2];
	}

	friend Vec3 operator+(const Vec3& a, const Vec3& b) {
		return Vec3(a.v_[0] + b.v_[0], a.v_[1] + b.v_[1], a.v_[2] + b.v_[2]);
	}

	friend Vec3 operator-(const Vec3& a, const Vec3& b) {
		return Vec3(a.v_[0] - b.v_[0], a.v_[1] - b.v_[1], a.v_[2] - b.v_[2]);
	}

	friend Vec3 operator*(const Vec3& a, const Vec3& b) {
		return Vec3(a.v_[0] * b.v_[0], a.v_[1] * b.v_[1], a.v_[2] * b.v_[2]);
	}

	friend Vec3 operator*(double t, const Vec3& a) {
		return Vec3(t * a.v_[0], t * a.v_[1], t * a.v_[2]);
	}

	friend Vec3 operator*(const Vec3& a, double t) {
		return t * a;
	}

	friend Vec3 operator/(const Vec3& a, double t) {
		return (1 / t) * a;
	}

	friend double dot(const Vec3& a, const Vec3& b) {
		return a.v_[0] * b.v_[0] + a.v_[1] * b.v_[1] + a.v_[2] * b.v_[2];
	}

	friend Vec3 cross(const Vec3& a, const Vec3& b) {
		return Vec3(
		    a.v_[1] * b.v_[2] - a.v_[2] * b.v_[1],
		    a.v_[2] * b.v_[0] - a.v_[0] * b.v_[2],
		    a.v_[0] * b.v_[1] - a.v_[1] * b.v_[0]
		);
	}

	friend  Vec3 unitVector(Vec3 v) {
		return v / v.length();
	}

	static  Vec3 random() {
		return Vec3(randomDouble(), randomDouble(), randomDouble());
	}

	static  Vec3 random(double min, double max) {
		return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
	}
};

inline Vec3 randomInUnitSphere() {
	for (;;) {
		auto p = Vec3::random(-1, 1);
		if (p.lengthSquared() >= 1) continue;
		return p;
	}
}

inline Vec3 randomUnitVector() {
	return unitVector(randomInUnitSphere());
}

inline Vec3 randomInHemisphere(const Vec3& normal) {
	Vec3 in_unit_sphere = randomInUnitSphere();
	if (dot(in_unit_sphere, normal) > 0.0)
		return in_unit_sphere;
	return -in_unit_sphere;
}

inline Vec3 reflect(const Vec3& v, const Vec3& normal) {
	return v - 2 * dot(v, normal) * normal;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double eta_quotient) {
	// uv = unit vector
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	Vec3 r_perpendicular = eta_quotient * (uv + cos_theta * n);
	Vec3 r_parallel = -std::sqrt(fabs(1.0 - r_perpendicular.lengthSquared())) * n;
	return r_perpendicular + r_parallel;
}

inline Vec3 randomInUnitDisk() {
	for (;;) {
		auto p = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
		if (p.lengthSquared() >= 1) continue;
		return p;
	}
}

using Color = Vec3;
