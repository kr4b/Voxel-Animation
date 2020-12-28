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

class Spline {
public:
	Spline(const gl::GLapi*);

	void render(const gl::GLapi*, const mat44f, const mat44f);

	void init_program(const gl::GLapi*);
	void init_vao(const gl::GLapi*);

	void update_from_screen_coords(const gl::GLapi*, const vec2f, const mat44f, const vec3f);

	void update_buffers(const gl::GLapi* gl);

	vec3f position_on_spline(const float t);

private:
	vec3f a, b, c, d;
	gl::Program debugProgram;
	gl::GL::UInt debugVao;
	gl::GL::UInt buffers[2];

};