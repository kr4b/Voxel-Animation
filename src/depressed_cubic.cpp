#include "depressed_cubic.hpp"

DepressedCubic::DepressedCubic(float a, float b, float c, float d) {
	this->p = (3.0f * a * c - b * b) / (3.0f * a * a);
	this->q = (2.0f * b * b * b - 9.0f * a * b * c + 27.0f * a * a * d) / (27.0f * a * a * a);

	this->discriminant = 27.0f * q * q + 4.0f * p * p * p;

    this->calculate_default_root();
}

float DepressedCubic::single_root() {
	if (discriminant > 0.0f) {
		const float D = sqrt(q * q / 4.0f + p * p * p / 27.0f);
		const float C0 = -0.5f * q + D;
		const float C1 = -0.5f * q - D;

		root = sign(C0) * pow(abs(C0), 1.0f / 3.0f) + sign(C1) * pow(abs(C1), 1.0f / 3.0f);
	}
	return root;
}

float DepressedCubic::first_root() {
	if (discriminant <= 0.0f) root = fac * cos(arccos);
	return root;
}

float DepressedCubic::second_root() {
	if (discriminant <= 0.0f) root = fac * cos(arccos - 2.0f / 3.0f * M_PI);
	return root;
}

float DepressedCubic::third_root() {
	if (discriminant <= 0.0f) root = fac * cos(arccos - 4.0f / 3.0f * M_PI);
	return root;
}