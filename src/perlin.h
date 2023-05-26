#pragma once

#include <array>

#include "common.h"

class perlin {
	static constexpr int point_count = 256;
public:
	perlin() {
		for (int i = 0; i < point_count; ++i)
			ranvec[i] = unit_vector(vec3::random(-1, 1)); // Non-uniform, test later
		perlin_generate_perm(perm_x);
		perlin_generate_perm(perm_y);
		perlin_generate_perm(perm_z);
	}

	double noise(const vec3& p) const {
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());
		auto i = static_cast<int>(floor(p.x()));
		auto j = static_cast<int>(floor(p.y()));
		auto k = static_cast<int>(floor(p.z()));
		vec3 c[2][2][2];

		for (int di = 0; di < 2; ++di)
			for (int dj = 0; dj < 2; ++dj)
				for (int dk = 0; dk < 2; ++dk)
					c[di][dj][dk] = ranvec[
						perm_x[(i + di) & 255] ^
						perm_y[(j + dj) & 255] ^
						perm_z[(k + dk) & 255]
					];

		return perlin_interp(c, u, v, w);
	}

	double turb(vec3 p, int depth = 7) const {
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
	static void perlin_generate_perm(std::array<int, point_count>& perm) {
		for (int i = 0; i < perm.size(); ++i)
			perm[i] = i;
		permute(perm);
	}

	static void permute(std::array<int, point_count>& perm) {
		for (int i = perm.size() - 1; i > 0; --i) {
			int target = random_int(0, i);
			std::swap(perm[i], perm[target]);
		}
	}

	static double perlin_interp(vec3 c[2][2][2], double u, double v, double w) {
		// Hermitian Smoothing
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);
		// Modified trilinear interpolation
		auto accum = 0.0;
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				for (int k = 0; k < 2; ++k) {
					vec3 weight(u - i, v - j, w - k);
					accum +=
						(i * uu + (1.0 - i) * (1.0 - uu)) *
						(j * vv + (1.0 - j) * (1.0 - vv)) *
						(k * ww + (1.0 - k) * (1.0 - ww)) *
						dot(c[i][j][k], weight);
				}
		return accum;
	}

private:
	std::array<vec3, point_count> ranvec;
	std::array<int, point_count> perm_x;
	std::array<int, point_count> perm_y;
	std::array<int, point_count> perm_z;
};