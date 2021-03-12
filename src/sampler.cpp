#include "sampler.hpp"

template <typename T>
Sampler<T>::Sampler(
    const AABB samplerAABB,
    const AABB realAABB,
    const float size,
    const std::vector<bool> sampler,
    const std::vector<T> data,
    const std::vector<vec3f> colors,
    const std::function<Spline(Ray, T, vec3f)> make_spline) :
    samplerAABB(samplerAABB), realAABB(realAABB), sampler(sampler), data(data), colors(colors), make_spline(make_spline), size(size) {}

template <typename T>
std::optional<Spline> Sampler<T>::get(const Ray ray, const vec3f samplePos) const {
    const float x = round((size - 1) * samplePos.x);
    const float y = round((size - 1) * samplePos.y);
    const float z = round((size - 1) * samplePos.z);

    const float index = z * size * size + y * size + x;
    if (index < 0 || index >= data.size() || !sampler[index]) {
        return std::nullopt;
    }

    return std::make_optional(make_spline(ray, data[index], colors[index]));
}

template <typename T>
void Sampler<T>::render(const gl::GLapi* gl) {
}
