#pragma once

#include <cmath>
#include <functional>
#include <optional>
#include <vector>

#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;

#include <flux/gl/glapi.hpp>
#include <flux/gl/checkpoint.hpp>
#include <flux/gl/setup/program.hpp>
namespace gl = flux::gl;

#include "aabb.hpp"
#include "spline.hpp"
#include "ray.hpp"

template <typename T>
class Sampler {
public:
    const AABB samplerAABB;
    const AABB realAABB;
    const gl::GLapi* gl_;

    Sampler(const AABB, const AABB, const float, const std::vector<unsigned char>, const std::vector<T>, const std::vector<vec3f>, const std::function <Spline(Ray, T, vec3f)>, const gl::GLapi*);

    std::optional<Spline> get(const Ray, const vec3f) const;

    void prepare(gl::GL::UInt, int);

    const std::vector<unsigned char> sampler;
    const std::vector<T> data;
    const std::vector<vec3f> colors;
    const std::function<Spline(Ray, T, vec3f)> make_spline;
    const size_t size;

    gl::GL::UInt sampler_texture;
    std::vector<gl::GL::UInt> data_textures;

    void create_sampler_texture();
    void create_data_texture(const int, const int, const int, const void *);

private:
    gl:: GL::UInt create_texture(const int, const int, const int, const int, const void *);

};
