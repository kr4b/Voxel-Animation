#pragma once

#include <flux/gl/glapi.hpp>
#include <flux/gl/checkpoint.hpp>
#include <flux/gl/setup/program.hpp>
namespace gl = flux::gl;

#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;

#include <algorithm>
#include <iostream>
#include <vector>

#include "math_util.hpp"

const int detail = 100;
const vec3f EPSILON = fml::make_vector<vec3f>(1.0e-6f, 1.0e-6f, 1.0e-6f);
const vec3f ZERO_VECTOR = fml::make_zero<vec3f>();
const vec3f ONE_VECTOR = fml::make_one<vec3f>();
const vec3f MAX_VALUE = fml::make_vector<vec3f>(1.0e6f, 1.0e6f, 1.0e6f);
const vec3f MIN_VALUE = -MAX_VALUE;

class Spline {
public:
	Spline(const gl::GLapi*);

	void render(const gl::GLapi*, const mat44f, const mat44f);

	void init_program(const gl::GLapi*);
	void init_vao(const gl::GLapi*);

	void update_from_screen_coords(const gl::GLapi*, const vec2f, const mat44f, const vec3f, const vec3f, const vec3f);

	void update_buffers(const gl::GLapi* gl);

	vec3f position_on_spline(const float t);

	void intersect_spline_aabb(const vec3f, const vec3f);

private:
	bool intersection;
	vec3f worldEntry, worldExit;
	vec2f lastCursorPos;
	vec3f a, b, c, d;
    vec3f start;
	gl::Program debugProgram;
	gl::GL::UInt lineVao, pointsVao;
	gl::GL::UInt buffers[4];
    std::vector<vec3f> points;

	vec3f intersected_aabb(const vec3f t, vec3f aAABBMin, vec3f aAABBMax);
	void calculate_near_far(const vec3f, const vec3f, const vec3f, const vec3f, vec2f*);
};

float point_in_aabb(const vec3f, const vec3f, const vec3f);