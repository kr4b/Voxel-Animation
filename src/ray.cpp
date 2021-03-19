#include "ray.hpp"
#include "sampler.hpp"

Ray::Ray(const vec3f origin, const vec3f dir, const gl::GLapi* gl) : origin(origin), dir(dir) {
    this->init_vao(gl);
}

void Ray::clean(const gl::GLapi* gl) {
    gl->deleteVertexArrays(1, &this->vao);
    gl->deleteBuffers(2, this->buffers);
}

void Ray::init_vao(const gl::GLapi* gl) {
    gl->genVertexArrays(1, &this->vao);
    gl->bindVertexArray(this->vao);

    gl->genBuffers(2, this->buffers);

    // Vertices
    gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[0]);
    gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * 2 * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
    gl->vertexAttribPointer(0, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
    gl->enableVertexAttribArray(0);

    // Colors
    gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[1]);
    gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * 2 * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
    gl->vertexAttribPointer(1, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
    gl->enableVertexAttribArray(1);

    gl->bindVertexArray(0);
}

void Ray::update_buffers(const gl::GLapi* gl) {
    gl::GL::Float vertices[2 * 3];
    gl::GL::Float colors[2 * 3];

    const vec3f from = this->origin;
    const vec3f to = from + this->dir;
    vertices[0] = from.x; vertices[1] = from.y; vertices[2] = from.z;
    vertices[3] = to.x; vertices[4] = to.y; vertices[5] = to.z;

    colors[0] = 0.0f; colors[1] = 0.2f; colors[2] = 0.0f;
    colors[3] = 0.0f; colors[4] = 0.2f; colors[5] = 0.0f;

    gl->bindVertexArray(this->vao);

    gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[0]);
    gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 3 * 2 * sizeof(gl::GL::Float), vertices);
    gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[1]);
    gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 3 * 2 * sizeof(gl::GL::Float), colors);

    gl->bindVertexArray(0);
}

vec2f Ray::intersect_ray_aabb(const AABB aabb) const {
    const vec3f t1 = (aabb.min - this->origin) / this->dir;
    const vec3f t2 = (aabb.max - this->origin) / this->dir;

    const vec3f mins = fml::min(t1, t2);
    const vec3f maxs = fml::max(t1, t2);

    const float near = std::max(std::max(mins.x, mins.y), mins.z);
    const float far = std::min(std::min(maxs.x, maxs.y), maxs.z);
    return vec2f(near, far);
}

void Ray::render(const gl::GLapi* gl) {
    gl->bindVertexArray(this->vao);

    gl->lineWidth(3.0f);
    gl->drawArrays(gl::GL::LINE_STRIP, 0, 2);
    gl->lineWidth(1.0f);

    gl->bindVertexArray(0);
}

