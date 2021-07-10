#pragma once

#include <functional>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "aabb.hpp"
#include "spline.hpp"
#include "plane.hpp"

class SplineMap {
public:
    AABB aabb;
    Plane base;
    Spline spline;

    SplineMap(Plane, Spline);

    void load_uniforms(const GLuint);

    std::optional<glm::vec3> texture_coords(const glm::vec3);

    void render();

    const float sizeSquared;

private:
    std::vector<Spline> edgeSplines;
    Plane topBase;
};
