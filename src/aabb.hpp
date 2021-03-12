#pragma once

#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;

class AABB {
public:
    const vec3f min, max;

    AABB(const vec3f& min, const vec3f& max) : min(fml::min(min, max)), max(fml::max(min, max)) {}
    inline vec3f size() const { return max - min; }
};

