#pragma once

#include <functional>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "aabb.hpp"
#include "spline_chain.hpp"
#include "plane.hpp"

class SplineMap {
public:
    Plane base;
    SplineChain splineChain;
    AABB aabb;
    const float sizeSquared;

    SplineMap(const Plane&, const SplineChain&);

    std::optional<glm::vec3> texture_coords(const glm::vec3) const;

    void render();
    void clean();

private:
    SplineChain edgeSplines[3];
    Plane topBase;
};
