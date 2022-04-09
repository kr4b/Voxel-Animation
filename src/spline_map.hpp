#pragma once

#include <functional>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "spline.hpp"
#include "plane.hpp"

class SplineMap {
public:
    Plane base;
    Spline spline;
    Plane topBase;
    Spline edgeSplines[3];
    float width, height, depth;

    SplineMap(const Plane&, const Spline&);

    // Transform given coordinate in shell space of the spline map to the coordinate in texture space
    std::optional<glm::vec3> texture_coords(const glm::vec3) const;

    void render();
    void clean();
};
