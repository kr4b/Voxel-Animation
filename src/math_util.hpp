#pragma once

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <glm/vec3.hpp>

// Step function based on GLSL step
inline glm::vec3 step(glm::vec3 edge, glm::vec3 x) {
	return glm::vec3(
		x.x >= edge.x ? 1.0f : 0.0f,
		x.y >= edge.y ? 1.0f : 0.0f,
		x.z >= edge.z ? 1.0f : 0.0f);
}

template <typename T> inline int sign(T x) {
	return (T(0) < x) - (x < T(0));
}