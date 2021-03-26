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

#include "math_util.hpp"

class Spline {
public:
	Spline(const gl::GLapi*);

	void render(const gl::GLapi*) const;
	void update_buffers(const gl::GLapi*);
    void clean(const gl::GLapi*);

	void update_from_screen_coords(const vec2f, const mat44f, const vec3f, const vec3f, const vec3f);
	void parameters_from_tangents(const vec3f, const vec3f, const vec3f, const vec3f);
	void parameters_from_points(const vec3f, const vec3f, const vec3f, const vec3f);

	vec3f position_on_spline(const float t);

	void intersect_spline_aabb(const vec3f, const vec3f);

    void set_color(const vec3f);

private:
	bool intersection;
	vec3f worldEntry, worldExit;
	vec2f lastCursorPos;
	vec3f a, b, c, d;
    vec3f start;
    vec3f color;
	gl::GL::UInt lineVao, pointsVao;
	gl::GL::UInt buffers[4];

    void init_vao(const gl::GLapi*);

	vec3f intersected_aabb(const vec3f t, vec3f aAABBMin, vec3f aAABBMax);
	bool calculate_near_far(const vec3f, const vec3f, const vec3f, const vec3f, vec2f*);
};

float point_in_aabb(const vec3f, const vec3f, const vec3f);