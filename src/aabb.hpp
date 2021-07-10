#pragma once

#include <glm/common.hpp>
#include <glm/vec3.hpp>

class AABB {
public:
    const glm::vec3 min, max;

    AABB(const glm::vec3& min, const glm::vec3& max) : min(glm::min(min, max)), max(glm::max(min, max)) {}
    inline glm::vec3 size() const { return max - min; }
};
