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

class Spline {
public:
	Spline(const gl::GLapi*);

	void render(const gl::GLapi*, const mat44f, const mat44f);

	void init_program(const gl::GLapi*);
	void init_vao(const gl::GLapi*);

	void update_from_screen_coords(const gl::GLapi*, const vec2f, const mat44f, const vec3f);

	void update_buffers(const gl::GLapi* gl);

	vec3f position_on_spline(const float t);

	void intersect_spline_aabb(const vec3f, const vec3f);

private:
	bool intersection;
	vec3f worldEntry, worldExit;
	vec3f a, b, c, d;
	gl::Program debugProgram;
	gl::GL::UInt lineVao, pointsVao;
	gl::GL::UInt buffers[4];

};

float depressed_cubic(float, float, float, float);
float point_in_aabb(const vec3f aPoint, const vec3f aAABBMin, const vec3f aAABBMax);
vec3f intersected_aabb(const vec3f t, Spline* aSpline, vec3f aAABBMin, vec3f aAABBMax);