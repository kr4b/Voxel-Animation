#pragma once

#include <optional>

#include <GL/glew.h>

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
    std::optional<std::pair<glm::ivec3, float>> walk_spline_map(SplineMap&, const Volume&, const float);

    void update_buffers();
    void render();
    void clean();

private:
    GLuint vao;
    GLuint buffers[2];

    void init_vao();
};
