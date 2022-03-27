#pragma once

#include <algorithm>

#include "math_util.hpp"

// Depressed cubic wrapper for solving real roots of cubic equations
// https://en.wikipedia.org/wiki/Cubic_equation#Depressed_cubic
class DepressedCubic {
public:
	DepressedCubic(float, float, float, float);

	float single_root();
	float first_root();
	float second_root();
	float third_root();

private:
	float p, q, discriminant, fac, root, arccos;

	void calculate_default_root();
};