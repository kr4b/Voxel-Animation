#include "sampler.hpp"

template <typename T>
Sampler<T>::Sampler(
    const AABB samplerAABB,
    const AABB realAABB,
    const float size,
    const std::vector<bool> sampler,
    const std::vector<T> data,
    const std::vector<vec3f> colors,
    const std::function<Spline(Ray, T, vec3f)> make_spline,
    const gl::GLapi* gl) :
    samplerAABB(samplerAABB), realAABB(realAABB), sampler(sampler), data(data), colors(colors), make_spline(make_spline), size(size) {

    this->init_vao(gl);
}

template <typename T>
void Sampler<T>::init_vao(const gl::GLapi* gl) {
    gl->genVertexArrays(1, &this->vao);
    gl->bindVertexArray(this->vao);

    gl->genBuffers(2, this->buffers);

    // Vertices
    gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[0]);
    gl->bufferData(gl::GL::ARRAY_BUFFER, 6 * 4 * 3 * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
    gl->vertexAttribPointer(0, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
    gl->enableVertexAttribArray(0);

    // Colors
    gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[1]);
    gl->bufferData(gl::GL::ARRAY_BUFFER, 6 * 4 * 3 * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
    gl->vertexAttribPointer(1, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
    gl->enableVertexAttribArray(1);
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
