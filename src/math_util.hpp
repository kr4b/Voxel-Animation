#pragma once

#include <flux/fml/stdtypes.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;

const double M_PI = 3.14159265;

inline vec3f step(vec3f edge, vec3f x) {
	return fml::make_vector<vec3f>(
		x.x >= edge.x ? 1.0f : 0.0f,
		x.y >= edge.y ? 1.0f : 0.0f,
		x.z >= edge.z ? 1.0f : 0.0f);
}

template <typename T> inline int sign(T x) {
	return (T(0) < x) - (x < T(0));
}