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
    const gl::GLapi* gl_) :
    samplerAABB(samplerAABB), realAABB(realAABB), size(size), sampler(sampler), data(data), colors(colors), make_spline(make_spline), gl_(gl_) {

    std::vector<float> rawColors;
    rawColors.reserve(colors.size() * 3);
    for (const vec3f& c : colors) {
        rawColors.push_back(c.x);
        rawColors.push_back(c.y);
        rawColors.push_back(c.z);
    }
}

template <typename T>
void Sampler<T>::create_sampler_texture() {
    const int internal_format = gl::GL::R8UI;
    const int format = gl::GL::RED_INTEGER;
    const int type = gl::GL::UNSIGNED_BYTE;

    sampler_texture = create_texture(internal_format, format, type, sampler.data());
}

template <typename T>
void Sampler<T>::create_data_texture(const int internal_format, const int format, const int type, const void *data) {
    gl::GL::UInt texture;
    
    texture = create_texture(internal_format, format, type, data);

    data_textures.push_back(texture);
}

template <typename T>
gl::GL::UInt Sampler<T>::create_texture(const int internal_format, const int format, const int type, const void *data) {
    gl::GL::UInt texture;

    this->gl_->activeTexture(gl::GL::TEXTURE1);
    this->gl_->createTextures(gl::GL::TEXTURE_3D, 1, &texture);
    this->gl_->bindTexture(gl::GL::TEXTURE_3D, texture);
    this->gl_->texImage3D(gl::GL::TEXTURE_3D, 0, internal_format, size, size, size, 0, format, type, data);
    this->gl_->texParameteri(gl::GL::TEXTURE_3D, gl::GL::TEXTURE_MIN_FILTER, gl::GL::NEAREST);
    this->gl_->texParameteri(gl::GL::TEXTURE_3D, gl::GL::TEXTURE_MAG_FILTER, gl::GL::NEAREST);
    this->gl_->bindTexture(gl::GL::TEXTURE_3D, 0);

    return texture;
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

template<typename T>
void Sampler<T>::prepare(gl::GL::UInt program, int active_offset) {
    this->gl_->activeTexture(gl::GL::TEXTURE0 + active_offset);
    this->gl_->bindTexture(gl::GL::TEXTURE_3D, sampler_texture);
    this->gl_->uniform1i(this->gl_->getUniformLocation(program, "sampler.dataCheck"), active_offset);

    int index = 0;
    for (gl::GL::UInt data_tex : data_textures) {
        active_offset += 1;

        this->gl_->activeTexture(gl::GL::TEXTURE0 + active_offset);
        this->gl_->bindTexture(gl::GL::TEXTURE_3D, data_tex);

        std::string name = "sampler.data[" + std::to_string(index) + "]";
        this->gl_->uniform1i(this->gl_->getUniformLocation(program, name.c_str()), active_offset);

        index += 1;
    }

    this->gl_->uniform1i(this->gl_->getUniformLocation(program, "sampler.size"), size);
    this->gl_->uniform3fv(this->gl_->getUniformLocation(program, "sampler.aabb.min"), 1, samplerAABB.min.data());
    this->gl_->uniform3fv(this->gl_->getUniformLocation(program, "sampler.aabb.max"), 1, samplerAABB.max.data());
}