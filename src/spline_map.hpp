#pragma once

#include <functional>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "aabb.hpp"
#include "spline_chain.hpp"
#include "better_plane.hpp"

class SplineMap {
public:
    BetterPlane base;
    BetterPlane topBase;
    SplineChain splineChain;
    SplineChain edgeSplines[3];
    float width, height, depth;

    SplineMap(const BetterPlane&, const SplineChain&);

    std::optional<glm::vec3> texture_coords(const glm::vec3) const;

    void render();
    void clean();
};
