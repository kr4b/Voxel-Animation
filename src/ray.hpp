#pragma once

#include <optional>

#include <flux/gl/glapi.hpp>
#include <flux/gl/checkpoint.hpp>
#include <flux/gl/setup/program.hpp>
namespace gl = flux::gl;

#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;

class AABB;
class SplineMap;
class Volume;

class Ray {
public:
    const vec3f origin, dir;

    Ray(const vec3f, const vec3f, const gl::GLapi*);

    vec2f intersect_ray_aabb(const AABB&) const;
    std::optional<std::pair<vec3i, float>> walk_spline_map(SplineMap&, const Volume&, const float);

    void update_buffers(const gl::GLapi*);
    void render(const gl::GLapi*);
    void clean(const gl::GLapi*);

private:
    const size_t VOLUME_STEPS = 1024;
    gl::GL::UInt vao;
    gl::GL::UInt buffers[2];

    void init_vao(const gl::GLapi*);
};