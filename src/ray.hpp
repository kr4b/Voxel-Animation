#pragma once

#include <optional>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class AABB;
class SplineMap;
class Volume;

class Ray {
public:
    const glm::vec3 origin, dir;

    Ray(const glm::vec3, const glm::vec3, const gl::GLapi*);

    glm::vec2 intersect_ray_aabb(const AABB&) const;
    std::optional<std::pair<vec3i, float>> walk_spline_map(SplineMap&, const Volume&, const float);

    void update_buffers(const gl::GLapi*);
    void render(const gl::GLapi*);
    void clean(const gl::GLapi*);

private:
    gl::GL::UInt vao;
    gl::GL::UInt buffers[2];

    void init_vao(const gl::GLapi*);
};
