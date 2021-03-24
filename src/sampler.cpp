#include "sampler.hpp"

template <typename T>
Sampler<T>::Sampler(
    const AABB samplerAABB,
    const AABB realAABB,
    const float size,
    const std::vector<unsigned char> sampler,
    const std::vector<T> data,
    const std::vector<vec3f> colors,
    const std::function<Spline(Ray, T, vec3f)> make_spline,
    const gl::GLapi* gl) :
    samplerAABB(samplerAABB), realAABB(realAABB), size(size), sampler(sampler), data(data), colors(colors), make_spline(make_spline) {

    std::vector<float> rawColors;
    rawColors.reserve(colors.size() * 3);
    for (const vec3f& c : colors) {
        rawColors.push_back(c.x);
        rawColors.push_back(c.y);
        rawColors.push_back(c.z);
    }
}

template <typename T>
std::optional<Spline> Sampler<T>::get(const Ray ray, const vec3f samplePos) const {
    const float x = round((this->size - 1) * samplePos.x);
    const float y = round((this->size - 1) * samplePos.y);
    const float z = round((this->size - 1) * samplePos.z);

    const float index = z * this->size * this->size + y * this->size + x;
    if (index < 0 || index >= this->data.size() || !sampler[index]) {
        return std::nullopt;
    }

    return std::make_optional(this->make_spline(ray, this->data[index], this->colors[index]));
}

template <typename T>
void Sampler<T>::render(const gl::GLapi* gl) {
}
