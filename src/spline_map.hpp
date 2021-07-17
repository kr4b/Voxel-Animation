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
    Plane base;
    Spline spline;
    AABB aabb;
    float threshold = 0.25f;
    float stepSize = 0.025f;
    const float sizeSquared;

    SplineMap(const Plane&, const Spline&);

    std::optional<glm::vec3> texture_coords(const glm::vec3) const;

    void render();
    void clean();

private:
    std::vector<Spline> edgeSplines;
    Plane topBase;
};
