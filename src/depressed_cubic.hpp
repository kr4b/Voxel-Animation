#pragma once

#include <algorithm>
#include <cmath>

#include "math_util.hpp"

class DepressedCubic {
public:
	DepressedCubic(float, float, float, float);

	float single_root();
	float first_root();
	float second_root();
	float third_root();

private:
	float p, q, discriminant, fac, root, arccos;

	void calculate_default_root() {
		if (discriminant > 0.0f) {
			root = single_root();
		}
		else {
			fac = 2.0f * sqrtf(-p / 3.0f);
			arccos = acosf(3.0f * q / (2.0f * p) * sqrtf(-3.0f / p)) / 3.0f;

			root = second_root();
		}
	}
};