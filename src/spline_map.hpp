#pragma once

#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;

#include "aabb.hpp"
#include "plane.hpp"
#include "spline.hpp"

class SplineMap {
public:
    const AABB aabb;
    const Plane base;
    const Spline spline;

    SplineMap(const Plane, const Spline);

    std::optional<vec3f> texture_coords(const vec3f) const;

    void render(const gl::GLapi*);

private:
    const float sizeSquared;
};

