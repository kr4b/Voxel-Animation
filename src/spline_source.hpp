#pragma once

#include <flux/gl/glapi.hpp>
#include <flux/gl/checkpoint.hpp>
#include <flux/gl/setup/program.hpp>
namespace gl = flux::gl;

#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;

#include <vector>

#include "spline.hpp"
#include "sampler.hpp"

class SplineSource {
public:
    void render(bool, const gl::GLapi*);

    void update_from_screen_coords(const vec2f, const mat44f, const vec3f);

    void update_buffers(const gl::GLapi*);
    void intersect_spline_aabb(const vec3f, const vec3f);

    void set_values(const gl::GLapi*, const size_t, const float);

    void clean(const gl::GLapi*);

    template <typename T>
    void update_from_screen_coords(const gl::GLapi* gl, const Sampler<T>& sampler) {
        const vec2f startCoords = this->coords - fml::make_splat<vec2f>(splineDist * (splineCount / 2));

        this->clean(gl);

        for (size_t i = 0; i < this->splineCount; i++) {
            vec2f currentCoords = startCoords + fml::make_vector<vec2f>(0.0f, float(i) * splineDist);
            for (size_t j = 0; j < this->splineCount; j++) {
                const vec2f transformed = currentCoords * 2.0f - fml::make_vector<vec2f>(1.0f, 1.0f);
                const vec4f hray = fml::make_vector<vec4f>(transformed.x, transformed.y, 1.0f, 1.0f);
                const vec4f wray = this->inverseProjCamera * hray;

                const vec3f origin = this->cameraWorldPos;
                const vec3f direction = fml::normalize(fml::make_vector<vec3f>(wray.x, wray.y, wray.z) / wray.w - origin) * fml::length(origin) * 2.0f;

                const Ray ray(origin, direction, gl);
                rays.push_back(ray);
                const std::optional<Spline> spline = ray.intersect_ray_sampler(sampler);
                if (spline.has_value()) {
                    splines.push_back(spline.value());
                }
                currentCoords.x += splineDist;
            }
        }
    }

private:
    vec2f coords;
    vec3f cameraWorldPos;
    mat44f inverseProjCamera;
    size_t splineCount;
    float splineDist;
    std::vector<Ray> rays;
    std::vector<Spline> splines;
};

