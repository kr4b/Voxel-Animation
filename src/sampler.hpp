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

#include "aabb.hpp"
#include "spline.hpp"
#include "ray.hpp"

template <typename T>
class Sampler {
public:
    const AABB samplerAABB;
    const AABB realAABB;

    Sampler(const AABB, const AABB, const float, const std::vector<bool>, const std::vector<T>, const std::vector<vec3f>, const std::function <Spline(Ray, T, vec3f)>, const gl::GLapi*);

    std::optional<Spline> get(const Ray, const vec3f) const;

    void render(const gl::GLapi*);

    const std::vector<bool> sampler;
private:
    const std::vector<T> data;
    const std::vector<vec3f> colors;
    const std::function<Spline(Ray, T, vec3f)> make_spline;
    const size_t size;
    gl::GL::UInt vao;
    gl::GL::UInt buffers[2];

    void init_vao(const gl::GLapi*);
};
