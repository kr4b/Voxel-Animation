#pragma once
#include <algorithm>

#include "math_util.hpp"

class DepressedCubic {
public:
	DepressedCubic(float, float, float, float);

	float find_roots();

	static float find_roots_static(float, float, float, float);

private:
	float p, q, discriminant;

	float single_root() {
		const float D = sqrt(q * q / 4.0f + p * p * p / 27.0f);
		const float C0 = -0.5f * q + D;
		const float C1 = -0.5f * q - D;

		return sign(C0) * pow(abs(C0), 1.0f / 3.0f) + sign(C1) * pow(abs(C1), 1.0f / 3.0f);
	}

	float first_root(float fac, float arccos) {
		return fac * cos(arccos);
	}

	float second_root(float fac, float arccos) {
		return fac * cos(arccos - 2.0f / 3.0f * M_PI);
	}

	float third_root(float fac, float arccos) {
		return fac * cos(arccos - 4.0f / 3.0f * M_PI);
	}
};