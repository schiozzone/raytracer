#pragma once

#include <array>

#include "math/common.hpp"
#include "math/vec3.hpp"

class Perlin {
	static constexpr int point_count = 256;

public:
	Perlin() {
		for (int i = 0; i < point_count; ++i)
			ranvec_[i] = unitVector(Vec3::random(-1, 1)); // Non-uniform, test later
		perlinGeneratePerm(perm_x_);
		perlinGeneratePerm(perm_y_);
		perlinGeneratePerm(perm_z_);
	}

	double noise(const Vec3& p) const {
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());
		auto i = static_cast<int>(floor(p.x()));
		auto j = static_cast<int>(floor(p.y()));
		auto k = static_cast<int>(floor(p.z()));
		Vec3 c[2][2][2];

		for (int di = 0; di < 2; ++di)
			for (int dj = 0; dj < 2; ++dj)
				for (int dk = 0; dk < 2; ++dk)
					c[di][dj][dk] = ranvec_[perm_x_[(i + di) & 255] ^ perm_y_[(j + dj) & 255] ^ perm_z_[(k + dk) & 255]];

		return perlinInterp(c, u, v, w);
	}

	double turb(Vec3 p, int depth = 7) const {
		auto accum = 0.0;
		auto weight = 1.0;
		for (int i = 0; i < depth; ++i) {
			accum += weight * noise(p);
			weight *= 0.5;
			p *= 2;
		}
		return fabs(accum);
	}

private:
	static void perlinGeneratePerm(std::array<int, point_count>& perm) {
		for (int i = 0; i < perm.size(); ++i)
			perm[i] = i;
		permute(perm);
	}

	static void permute(std::array<int, point_count>& perm) {
		for (int i = perm.size() - 1; i > 0; --i) {
			int target = randomInt(0, i);
			std::swap(perm[i], perm[target]);
		}
	}

	static double perlinInterp(Vec3 c[2][2][2], double u, double v, double w) {
		// Hermitian Smoothing
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);
		// Modified trilinear interpolation
		auto accum = 0.0;
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				for (int k = 0; k < 2; ++k) {
					Vec3 weight(u - i, v - j, w - k);
					accum +=
					    (i * uu + (1.0 - i) * (1.0 - uu)) *
					    (j * vv + (1.0 - j) * (1.0 - vv)) *
					    (k * ww + (1.0 - k) * (1.0 - ww)) *
					    dot(c[i][j][k], weight);
				}
		return accum;
	}

private:
	std::array<Vec3, point_count> ranvec_;
	std::array<int, point_count> perm_x_;
	std::array<int, point_count> perm_y_;
	std::array<int, point_count> perm_z_;
};
