#pragma once

#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;

#include "ray.hpp"

class Plane {
public:
    const vec3f center;
    const vec3f size;
    vec3f span1, span2;

    Plane(vec3f center, vec3f size);

    std::optional<vec2f> intersect(const Ray& ray) const;

private:
    vec3f normal;

};

