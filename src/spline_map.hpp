#pragma once

#include <functional>

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

    SplineMap(Plane, Spline, const gl::GLapi* gl);

    void load_uniforms(const gl::GL::UInt, const gl::GLapi*);

    std::optional<glm::vec3> texture_coords(const glm::vec3);

    void render(const gl::GLapi*);

private:
    float sizeSquared;
    std::vector<Spline> edgeSplines;
    Plane topBase;
};

