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

    Ray(const vec3f, const vec3f, const gl::GLapi*);

    vec2f intersect_ray_aabb(const AABB) const;

    void update_buffers(const gl::GLapi*);
    void render(const gl::GLapi*);
    void clean(const gl::GLapi*);

    template <typename T>
    std::optional<Spline> intersect_ray_sampler(const Sampler<T>& sampler) const {
        vec2f ts = intersect_ray_aabb(sampler.samplerAABB);

        if (ts.x <= ts.y && ts.y >= 0.0) {
            if (ts.x < 0.0) {
                ts.x = 0.0;
            }

            const vec3f worldEntry = this->origin + this->dir * ts.x;
            const vec3f worldExit = this->origin + this->dir * ts.y;

            const vec3f vscale = sampler.samplerAABB.size();
            const vec3f ventry = (worldEntry - sampler.samplerAABB.min) / vscale;
            const vec3f vexit = (worldExit - sampler.samplerAABB.max) / vscale;

            // Walk the ray from entry to exit to determine the intersection point
            for (size_t i = 0; i < VOLUME_STEPS; i++) {
                const float ii = float(i) / float(VOLUME_STEPS);

                const vec3f samplePos = ventry * (1.0f - ii) + vexit * ii;
                std::optional<Spline> spline = sampler.get(*this, ts.x, samplePos);

                if (spline) {
                    return spline;
                }
            }
        }

        return std::nullopt;
    }

private:
    const size_t VOLUME_STEPS = 1024;
    gl::GL::UInt vao;
    gl::GL::UInt buffers[2];

    void init_vao(const gl::GLapi*);
};