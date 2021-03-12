#include "ray.hpp"
#include "sampler.hpp"

Ray::Ray(const vec3f& origin, const vec3f& dir) : origin(origin), dir(dir) {}

vec2f Ray::intersect_ray_aabb(const AABB& aabb) {
    const vec3f t1 = (aabb.min - this->origin) / this->dir;
    const vec3f t2 = (aabb.max - this->origin) / this->dir;

    const vec3f mins = fml::min(t1, t2);
    const vec3f maxs = fml::max(t1, t2);

    const float near = std::max(std::max(mins.x, mins.y), mins.z);
    const float far = std::min(std::min(maxs.x, maxs.y), maxs.z);
    return vec2f(near, far);
}

template <typename T>
std::optional<Spline> Ray::intersect_ray_sampler(const Sampler<T>& sampler) {
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
            std::optional<Spline> spline = sampler.get(*this, samplePos);

            if (spline) {
                return spline;
            }
        }
    }

    return std::nullopt;
}

void Ray::render(const gl::GLapi* gl) {
}

