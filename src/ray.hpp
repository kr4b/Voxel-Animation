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
    std::optional<std::pair<glm::ivec3, float>> walk_spline_map(const SplineMap&, const Volume&, const float);

    void update_buffers(std::optional<std::pair<glm::ivec3, float>>, const glm::ivec3);
    void render() const;
    void clean();

private:
    GLuint lineVao, pointVao;
    GLuint buffers[4];
    bool intersect;

    void init_vao();
};
