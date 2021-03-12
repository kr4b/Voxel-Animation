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

#include <vector>

#include "spline.hpp"

class SplineSource {
public:
    void render(const gl::GLapi*);
    void update_from_screen_coords(const vec2f, const mat44f, const vec3f);
    void update_from_screen_coords(const vec3f, const vec3f);
    void update_buffers(const gl::GLapi* gl);
    void intersect_spline_aabb(const vec3f, const vec3f);

    void set_values(const gl::GLapi*, const size_t, const float);

private:
    vec2f coords;
    vec3f cameraWorldPos;
    mat44f inverseProjCamera;
    size_t splineCount;
    float splineDist;
    std::vector<Spline> splines;
};

