#pragma once

#include <optional>

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class AABB;
class SplineMap;
class Volume;

class Ray {
public:
    const glm::vec3 origin, dir;

    Ray(const glm::vec3, const glm::vec3);

    glm::vec2 intersect_ray_aabb(const AABB&) const;
    std::optional<std::pair<glm::ivec3, float>> walk_spline_map(const SplineMap&, const Volume&, const float, const float);

    void init_vao();
    void update_buffers(glm::vec2);
    void render(bool, bool) const;
    void clean();

    glm::vec2 intersect_ray_spline_map(const SplineMap&) const;
    std::optional<float> Ray::intersect_ray_line_segment(const glm::vec3, const glm::vec3) const;

private:
    GLuint lineVao, pointVao;
    GLuint buffers[4];
    bool intersect;
};
