#pragma once

#include <functional>

#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;

#include "aabb.hpp"
#include "spline.hpp"
#include "plane.hpp"

class SplineMap {
public:
    AABB aabb;
    Plane base;
    Spline spline;

    SplineMap(Plane, Spline, const gl::GLapi* gl);

    std::optional<vec3f> texture_coords(const vec3f);

    void render(const gl::GLapi*);

private:
    float sizeSquared;
    std::vector<Spline> edgeSplines;
    Plane topBase;
};

