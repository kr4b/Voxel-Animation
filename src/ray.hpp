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

#include "aabb.hpp"
#include "spline.hpp"

template <typename T>
class Sampler;

class Ray {
public:
    const vec3f origin, dir;

    Ray(const vec3f&, const vec3f&);

    vec2f intersect_ray_aabb(const AABB&);
    template <typename T>
    std::optional<Spline> intersect_ray_sampler(const Sampler<T>&);

    void render(const gl::GLapi*);

private:
    const size_t VOLUME_STEPS = 1024;
    gl::GL::UInt vao;
    gl::GL::UInt buffers[2];

    void init_vao(const gl::GLapi*);
};