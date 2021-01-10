#include "depressed_cubic.hpp"

DepressedCubic::DepressedCubic(float a, float b, float c, float d) {
	this->p = (3.0f * a * c - b * b) / (3.0f * a * a);
	this->q = (2.0f * b * b * b - 9.0f * a * b * c + 27.0f * a * a * d) / (27.0f * a * a * a);

	this->discriminant = 27.0f * q * q + 4.0f * p * p * p;
}

float DepressedCubic::find_roots() {
	if (discriminant > 0.0f) {
		return single_root();
	}
	else {
		const float fac = 2.0f * sqrt(-p / 3.0f);
		const float arccos = acos(3.0f * q / (2.0f * p) * sqrt(-3.0f / p)) / 3.0f;

		return second_root(fac, arccos);
	}
}

float DepressedCubic::find_roots_first(float a, float b, float c, float d) {
    DepressedCubic cubic(a, b, c, d);

    if (cubic.discriminant > 0.0f) {
        return cubic.single_root();
    }
    else {
        const float fac = 2.0f * sqrt(-cubic.p / 3.0f);
        const float arccos = acos(3.0f * cubic.q / (2.0f * cubic.p) * sqrt(-3.0f / cubic.p)) / 3.0f;

        return cubic.first_root(fac, arccos);
    }
}

float DepressedCubic::find_roots_second(float a, float b, float c, float d) {
    DepressedCubic cubic(a, b, c, d);

    if (cubic.discriminant > 0.0f) {
        return cubic.single_root();
    }
    else {
        const float fac = 2.0f * sqrt(-cubic.p / 3.0f);
        const float arccos = acos(3.0f * cubic.q / (2.0f * cubic.p) * sqrt(-3.0f / cubic.p)) / 3.0f;

        return cubic.second_root(fac, arccos);
    }
}

float DepressedCubic::find_roots_third(float a, float b, float c, float d) {
    DepressedCubic cubic(a, b, c, d);

    if (cubic.discriminant > 0.0f) {
        return cubic.single_root();
    }
    else {
        const float fac = 2.0f * sqrt(-cubic.p / 3.0f);
        const float arccos = acos(3.0f * cubic.q / (2.0f * cubic.p) * sqrt(-3.0f / cubic.p)) / 3.0f;

        return cubic.third_root(fac, arccos);
    }
}

float DepressedCubic::find_roots_static(float a, float b, float c, float d) {
	DepressedCubic cubic(a, b, c, d);
	return cubic.find_roots();
}