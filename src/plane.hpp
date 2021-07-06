#pragma once

#include <flux/gl/glapi.hpp>
#include <flux/gl/checkpoint.hpp>
#include <flux/gl/setup/program.hpp>
namespace gl = flux::gl;

#include <optional>
#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;

class Ray;

class Plane {
public:
    const vec3f center;
    const vec3f half_size;
    vec3f span1, span2;

    vec3f size;
    vec3f normal;

    vec3f min;
    vec3f max;

    mat44f matrix;
    mat44f inv_matrix;

    Plane(vec3f center, vec3f half_size);

    void init_vao(const gl::GLapi*, const vec3f);
    void render(const gl::GLapi*);
    void clean(const gl::GLapi*);

    Plane transform(const mat44f &matrix) {
        vec4f transformed = matrix * vec4f(this->center.x, this->center.y, this->center.z, 1.0);
        return Plane(
            vec3f(transformed.x, transformed.y, transformed.z),
            this->half_size
        );
    }

    std::optional<vec2f> intersect(const Ray&) const;
private:
    gl::GL::UInt vao;
    gl::GL::UInt buffers[2];
};

