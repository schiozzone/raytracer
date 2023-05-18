#pragma once

#include <cmath>
#include <iostream>

#include "random_number.h"

class vec3 {
public:
	vec3() : v{ 0,0,0 } {}
	vec3(double v0, double v1, double v2) : v{ v0,v1,v2 } {}

	double x() const { return v[0]; }
	double y() const { return v[1]; }
	double z() const { return v[2]; }

	vec3 operator-() const { return vec3(-v[0], -v[1], -v[2]); }
	double operator[](int i) const { return v[i]; }
	double& operator[](int i) { return v[i]; }

	vec3& operator+=(const vec3& a) {
		v[0] += a.v[0];
		v[1] += a.v[1];
		v[2] += a.v[2];
		return *this;
	}

	vec3& operator*=(const double t) {
		v[0] *= t;
		v[1] *= t;
		v[2] *= t;
		return *this;
	}

	vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	double length() const {
		return std::sqrt(length_squared());
	}

	double length_squared() const {
		return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	}

	bool near_zero() const {
		constexpr auto epsilon = 1e-8;
		return (fabs(v[0]) < epsilon) && (fabs(v[1]) < epsilon) && (fabs(v[2]) < epsilon);
	}

private:
	double v[3];

public:
	friend inline std::ostream& operator<<(std::ostream& out, const vec3& a) {
		return out << a.v[0] << ' ' << a.v[1] << ' ' << a.v[2];
	}

	friend inline vec3 operator+(const vec3& a, const vec3& b) {
		return vec3(a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2]);
	}

	friend inline vec3 operator-(const vec3& a, const vec3& b) {
		return vec3(a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2]);
	}

	friend inline vec3 operator*(const vec3& a, const vec3& b) {
		return vec3(a.v[0] * b.v[0], a.v[1] * b.v[1], a.v[2] * b.v[2]);
	}

	friend inline vec3 operator*(double t, const vec3& a) {
		return vec3(t * a.v[0], t * a.v[1], t * a.v[2]);
	}

	friend inline vec3 operator*(const vec3& a, double t) {
		return t * a;
	}

	friend inline vec3 operator/(const vec3& a, double t) {
		return (1 / t) * a;
	}

	friend inline double dot(const vec3& a, const vec3& b) {
		return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
	}

	friend inline vec3 cross(const vec3& a, const vec3& b) {
		return vec3(
			a.v[1] * b.v[2] - a.v[2] * b.v[1],
			a.v[2] * b.v[0] - a.v[0] * b.v[2],
			a.v[0] * b.v[1] - a.v[1] * b.v[0]
			);
	}

	friend inline vec3 unit_vector(vec3 v) {
		return v / v.length();
	}

	static inline vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	static inline vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
};

vec3 random_in_unit_sphere() {
	for (;;) {
		auto p = vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_Sphere = random_in_unit_sphere();
	if (dot(in_unit_Sphere, normal) > 0.0)
		return in_unit_Sphere;
	return -in_unit_Sphere;
}

vec3 reflect(const vec3& v, const vec3& normal) {
	return v - 2 * dot(v, normal) * normal;
}

vec3 refract(const vec3& uv, const vec3& n, double eta_quotient) {
	// uv = unit vector
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 r_perpendicular = eta_quotient * (uv + cos_theta * n);
	vec3 r_parallel = -std::sqrt(fabs(1.0 - r_perpendicular.length_squared())) * n;
	return r_perpendicular + r_parallel;
}

vec3 random_in_unit_disk() {
	for (;;) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

using color = vec3;
