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
#include <optional>

#include "math_util.hpp"
#include "plane.hpp"

class Spline {
public:
	vec3f a, b, c, d;

	Spline(const gl::GLapi*);
	Spline(const gl::GLapi*, const vec3f, const vec3f, const vec3f, const vec3f);

	void render() const;
	void update_buffers();
    void clean();

	void update_from_screen_coords(const vec2f, const mat44f, const vec3f, const vec3f, const vec3f);
	void parameters_from_tangents(const vec3f, const vec3f, const vec3f, const vec3f);
	void parameters_from_points(const vec3f, const vec3f, const vec3f, const vec3f);

	vec3f position_on_spline(const float t) const;

	std::optional<vec2f> intersect_spline_aabb(const vec3f, const vec3f);

    void set_color(const vec3f);

	std::vector<float> get_extremes();
	std::optional<float> intersect_spline_plane(const Plane&);

	Spline transform(const mat44f &matrix) const {
		vec4f ta = matrix * vec4f(this->a.x, this->a.y, this->a.z, 0.0f);
		vec4f tb = matrix * vec4f(this->b.x, this->b.y, this->b.z, 0.0f);
		vec4f tc = matrix * vec4f(this->c.x, this->c.y, this->c.z, 0.0f);
		vec4f td = matrix * vec4f(this->d.x, this->d.y, this->d.z, 1.0f);

		return Spline(
			this->gl,
			vec3f(ta.x, ta.y, ta.z),
			vec3f(tb.x, tb.y, tb.z),
			vec3f(tc.x, tc.y, tc.z),
			vec3f(td.x, td.y, td.z)
		);
	}

private:
	bool intersection;
	vec3f worldEntry, worldExit;
	vec2f lastCursorPos;
    vec3f start;
    vec3f color;
	gl::GL::UInt lineVao, pointsVao;
	gl::GL::UInt buffers[4];
	const gl::GLapi* gl;

    void init_vao();

	vec3f intersected_aabb(const vec3f t, vec3f aAABBMin, vec3f aAABBMax);
	bool calculate_near_far(const vec3f, const vec3f, const vec3f, const vec3f, vec2f*);
};

float point_in_aabb(const vec3f, const vec3f, const vec3f);